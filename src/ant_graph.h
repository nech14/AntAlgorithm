#ifndef ANT_GRAPH_H
#define ANT_GRAPH_H

#include <utility> 


using namespace std;

class AntGraph : public MyGraph {

public:
	
	double p = 0.001; //pheromone evaporation ∈ [0, 1]
	int a = 1; //the importance of pheromone.
	int b = 3; //the importance of path length.
	double q = 0.000001; //pheromone left by an ant

	double minChanceTurn = 0.000001;

	double start_r = 0.1;
	double** r_matrix; //pheromone matrix

	int count_ants = 51;
	Ant* ants;

	int count_repetitions = 1000000;

	bool work_with_alive_ants = true;

	/*friend ostream& operator<<(ostream& os, const AntGraph& obj) {
        os << "AntGraph: " << obj.countVertices << endl;

        int maxLen = 2;
        for (int i = 0; i < obj.countVertices; ++i) {
            for (int j = 0; j < obj.countVertices; ++j) {
                int len = to_string(obj.r_matrix[i][j]).length();
                maxLen = max(maxLen, len);
            }
        }

        for (int i = 0; i < obj.countVertices; ++i) {
            for (int j = 0; j < obj.countVertices; ++j) {
                os << setw(maxLen) << obj.r_matrix[i][j] << "  ";
            }
            os << endl;
        }
        return os;
    }*/

	AntGraph() : MyGraph() {
		create_r_matrix();
	}
	AntGraph(const string& fileName) : MyGraph(fileName) {
		create_r_matrix();
	}
	AntGraph(const string& fileName, bool bidirectional) : MyGraph(fileName, bidirectional) {
		create_r_matrix();
	}

	void create_r_matrix() {
		r_matrix = new double* [countVertices];
		for (int i = 0; i < countVertices; ++i) {
			r_matrix[i] = new double[countVertices];
		}

		for (int i = 0; i < countVertices; ++i) {
			for (int j = 0; j < countVertices; ++j) {
				if(i == j)
					r_matrix[i][j] = 0;
				else
					r_matrix[i][j] = start_r;
			}
		}
	}

	void create_ants(int start_node) {
		
		ants = new Ant[count_ants];

		for (int i = 0; i < count_ants; i++) {
			if (start_node < 0) {
				ants[i] = Ant(i % (countVertices), 0);
			}
			else {
				ants[i] = Ant(start_node, 0);
			}

		}

	
	}

	void delete_ants() {

		delete[] ants;
		ants = nullptr;
	
	}

	//numerator
	double weight_way(const int row, const int col) {
		
		double n = 1. / matrix[row][col];
		double r = r_matrix[row][col];
		//cout << "row:" << row << " col:" << col << '\n';
		//cout << "n: " << n << " r:" << r << " r2:" << pow(r, a) << " n2:" << pow(n, b) << " minChanceTurn:" << minChanceTurn << '\n';

		double Prc = pow(r, a) * pow(n, b) + minChanceTurn;

		//cout << "Prc:" << Prc << '\n';

		return Prc;
	
	}


	pair<vector<double>, double> weight_ways(const int row) {

		vector<double> weights_ways = vector<double>(); //vector of numerators
		double sum_weight = 0; //denominator
		double value = 0;
	
		for (int col = 0; col < this->countVertices; ++col) {

			if (row == col) continue;

			value = weight_way(row, col);
			weights_ways.push_back(value);
			sum_weight += value;

		}

		return make_pair(weights_ways, sum_weight);
	
	}


	pair<vector<double>, double> weight_ways(const int row, const vector<int>awailable_ways) {

		vector<double> weights_ways = vector<double>(); //vector of numerators
		double sum_weight = 0; //denominator
		double value = 0;

		int col;

		for (int i = 0; i < awailable_ways.size(); ++i) {

			col = awailable_ways[i];

			value = weight_way(row, col);
			/*cout << "value: " << value << '\n'*/;
			weights_ways.push_back(value);
			sum_weight += value;

		}

		return make_pair(weights_ways, sum_weight);

	}

	void pheromone_update_on_way(const int row, const int col) {
	
		double r_sum = 0;

		for (int id_ant = 0; id_ant < count_ants; ++id_ant) {
			
			if (work_with_alive_ants && !ants[id_ant].alive)
				continue;
			
			if (!ants[id_ant].passed_through_point(row, col))
				continue;

			r_sum += this->q / ants[id_ant].len;
			//r_sum += this->q ;

		}
		double r = (1 - this->p) * this->r_matrix[row][col] + r_sum;
		//cout << "r:" << r << " r_m:" << r_matrix[row][col];
		r_matrix[row][col] = r;
		//cout << " new_r_m:" << r_matrix[row][col] << '\n';

	}

	void update_r_matrix() {
	
		for (int row = 0; row < countVertices; row++) {
		
			for (int col = 0; col < countVertices; col++) {

				if (col == row)
					continue;

				pheromone_update_on_way(row, col);

			}

		}
	
	}

	bool work_with_one_ant(Ant& ant) {
		vector<int> availableEdges = getAvailableEdges(ant.last());
		vector<int> availableWays = ant.getAvailableWays(availableEdges);

		//cout << "availableEdges: " << availableEdges.size() << " availableWays:" << availableWays.size() << " | " << (availableEdges.size() <= 0 || availableWays.size() <= 0) << '\n';
		if (availableEdges.size() <= 0 || availableWays.size() <= 0)
			return false; //dead ant

		pair<vector<double>, double> result_weight_ways = weight_ways(ant.last(), availableWays);

		int id_selected_way = ant.choosingWay(result_weight_ways);
		//cout << "way_id: " << id_selected_way << "ways: " << availableWays.size() << '\n';
		//cout << "weights_ways: " << result_weight_ways.first << " sum_weight: " << result_weight_ways.second << '\n';
		int selected_way = availableWays[id_selected_way];



		ant.add_way(selected_way, matrix[ant.last()][selected_way]);

		return true; // alive ant
	}


	void antAlgorithm(int start_node = -1) {

		std::cout << "start\n";


		for (int r = 0; r < count_repetitions; r++){	

			//cout << "create_ant" << '\n';
			create_ants(start_node);

			//cout << "work" << '\n';
			for (int step = 0; step < countVertices; step++) {

				for (int id_ant = 0; id_ant < count_ants; id_ant++) {
					//std::cout << "step:" << step << " id_ant:" << id_ant << "\n";
					Ant& ant = ants[id_ant];
					if (!ant.alive)
						continue;

					ant.alive = work_with_one_ant(ant);
				
				}			

			}

			//cout << "update_matrix" << '\n';
			update_r_matrix();
			//cout << *this;

			/*for (int id_ant = 0; id_ant < count_ants; id_ant++) {
				std::cout << "ant_" << id_ant << ":" << ants[id_ant] << "\n";
			}*/

			//cout << "save" << '\n';
			saveResult();
			//cout << "delete_ant" << '\n';
			delete_ants();
		}
		return;
	}

	void saveResult(string filename = "result/output.txt", int countNode=51) {

		ofstream file(filename, ios::app);


		if (file.is_open()) {

			for (int id_ant = 0; id_ant < count_ants; id_ant++) {

				if (ants[id_ant].alive) {

					if(ants[id_ant].start_note == ants[id_ant].last()){
						file << ants[id_ant].size() +1 << ",";
						file << ants[id_ant].start_note << ",";
						for (int w = 0; w < ants[id_ant].size(); w++) {

							file << ants[id_ant].way[w] << ",";

						}

						file << ants[id_ant].len << '\n';
					}
				
				}
			
			}

			file.close(); 
			cout << "Data written to " << filename << "\n";
		}
		else {
			cout << "Failed to open " << filename << "\n";
		}


	}

};




#endif