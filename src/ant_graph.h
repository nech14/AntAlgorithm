#ifndef ANT_GRAPH_H
#define ANT_GRAPH_H

#include <utility> 
#include "../dependencies\include\nlohmann\json.hpp"
#include <future>

using namespace std;

class AntGraph : public MyGraph {

public:
	
	double p = 0.1; //pheromone evaporation ∈ [0, 1]
	int a = 2; //the importance of pheromone.
	int b = 3; //the importance of path length.
	double q = 1.; //pheromone left by an ant

	double minChanceTurn = 0.0001;

	double start_r = 0.1;
	double** r_matrix; //pheromone matrix

	int count_ants = 51;
	Ant* ants;

	int count_repetitions = 1000000;

	bool work_with_alive_ants = true;
	int start_node = -1;

	bool logs = true;

	bool async_mode_matrix = false;
	bool async_mode_ants = false;


	AntGraph() : MyGraph() {
		create_r_matrix();
	}
	/*AntGraph(const string& fileName) : MyGraph(fileName) {
		create_r_matrix();
	}*/
	AntGraph(const string& jsonFileName) : MyGraph(jsonFileName) {
		nlohmann::json objJson;

		fstream fileInput;
		fileInput.open(jsonFileName);

		fileInput >> objJson;

		fileInput.close();

		a = objJson["a"];
		b = objJson["b"];
		p = objJson["p"];
		q = objJson["q"];
		start_r = objJson["start_r"];
		minChanceTurn = objJson["minChanceTurn"];
		count_repetitions = objJson["count_repetitions"];
		count_ants = objJson["count_ants"];
		start_node = objJson["start_node"];
		work_with_alive_ants = objJson["work_with_alive_ants"];
		logs = objJson["logs"];
		async_mode_matrix = objJson["async_mode_matrix"];
		async_mode_ants = objJson["async_mode_ants"];

		create_r_matrix();
	}
	AntGraph(const string& fileName, bool bidirectional) : MyGraph(fileName, bidirectional) {
		create_r_matrix();
	}

	void create_r_matrix() {
		r_matrix = new double* [countVertices];
		for (int i = 0; i < countVertices; i++) {
			r_matrix[i] = new double[countVertices];
		}

		for (int i = 0; i < countVertices; i++) {
			for (int j = 0; j < countVertices; j++) {
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

		double Prc = pow(r, a) * pow(n, b) + minChanceTurn;

		return Prc;
	
	}


	pair<vector<double>, double> weight_ways(const int row) {

		vector<double> weights_ways = vector<double>(); //vector of numerators
		double sum_weight = 0.; //denominator
		double value = 0.;
	
		for (int col = 0; col < this->countVertices; col++) {

			if (row == col) continue;

			value = weight_way(row, col);
			weights_ways.push_back(value);
			sum_weight += value;

		}

		return make_pair(weights_ways, sum_weight);
	
	}


	pair<vector<double>, double> weight_ways(const int row, const vector<int>awailable_ways) {

		vector<double> weights_ways = vector<double>(); //vector of numerators
		double sum_weight = 0.; //denominator
		double value = 0.;
		 
		int col;

		for (int i = 0; i < awailable_ways.size(); i++) {

			col = awailable_ways[i];

			value = weight_way(row, col);

			weights_ways.push_back(value);
			sum_weight += value;

		}

		return make_pair(weights_ways, sum_weight);

	}

	void pheromone_update_on_way(const int row, const int col) {
	
		double r_sum = 0.;

		for (int id_ant = 0; id_ant < count_ants; id_ant++) {
			
			if (work_with_alive_ants && !ants[id_ant].alive)
				continue;

			if (!ants[id_ant].passed_through_point(row, col))
				continue;
			
			r_sum += q / ants[id_ant].len;
			
		}
		double r = (1. - p) * r_matrix[row][col] + r_sum;
		r_matrix[row][col] = r;

	}

	void update_r_matrix() {
		
		vector<future<void>> futures;
		for (int row = 0; row < countVertices; row++) {
		
			for (int col = 0; col < countVertices; col++) {

				if (col == row)
					continue;

				if (async_mode_matrix) {
					futures.push_back(async(launch::async, [this, row, col]() {
						pheromone_update_on_way(row, col);
						}));
				}
				else {
					pheromone_update_on_way(row, col);
				}

			}

		}

		if (async_mode_matrix) {
		
			for (auto& future : futures) {
				future.get();
			}

		}

	
	}

	void work_with_one_ant(Ant& ant) {
		if (!ant.alive)
			return;

		vector<int> availableEdges = getAvailableEdges(ant.last());
		vector<int> availableWays = ant.getAvailableWays(availableEdges);

		if (availableEdges.size() <= 0 || availableWays.size() <= 0){
			ant.alive = false;
			return; //dead ant
		}

		pair<vector<double>, double> result_weight_ways = weight_ways(ant.last(), availableWays);

		int id_selected_way = ant.choosingWay(result_weight_ways);
		int selected_way = availableWays[id_selected_way];


		ant.amount_pheromone_on_way += r_matrix[ant.last()][selected_way];
		ant.chance_way *= result_weight_ways.first[id_selected_way] / result_weight_ways.second;
		ant.add_way(selected_way, matrix[ant.last()][selected_way]);

		return; // alive ant
	}


	void antAlgorithm() {

		cout << "start\n";


		for (int r = 0; r < count_repetitions; r++){	

			create_ants(start_node);

			for (int step = 0; step < countVertices; step++) {
				
				vector<future<void>> futures;
				

				for (int id_ant = 0; id_ant < count_ants; id_ant++) {
					Ant& ant = ants[id_ant];			
					

					if (async_mode_ants) {
						futures.push_back(async(launch::async, [this, &ant]() -> void {
							return work_with_one_ant(ant);
							}));

					}

					else {
						work_with_one_ant(ant);
					}
				
				}		

				if (async_mode_ants) {
					for (auto& future : futures) {
						future.get();
					}
				}

			}

			update_r_matrix();
			saveResult();
			delete_ants();
		}
		return;
	}


	double sum_r_matrix() {

		double r_sum = 0;

		for (int r = 0; r < countVertices; r++) {

			for (int c = 0; c < countVertices; c++) {

				r_sum += r_matrix[r][c];

			}

		}

		return r_sum;

	}


	void saveResult(string filename = "result/output.txt", int countNode=-1, bool only_alive=false) {

		ofstream file(filename, ios::app);

		if (countNode <= 0)
			countNode = countVertices;

		double r_sum = sum_r_matrix();

		if (file.is_open()) {

			for (int id_ant = 0; id_ant < count_ants; id_ant++) {

				if (ants[id_ant].alive || (only_alive && ants[id_ant].size() == countNode)) {

					if(ants[id_ant].start_note == ants[id_ant].last()){
						file << ants[id_ant].chance_way << ",";
						file << ants[id_ant].amount_pheromone_on_way / r_sum << ",";
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
			if (logs)
				cout << "Data written to " << filename << "\n";
		}
		else {
			if (logs)
				cout << "Failed to open " << filename << "\n";
		}


	}


};




#endif