#ifndef ANT_H
#define ANT_H

#include <random>

using namespace std;

class Ant : public Way {

public:

	bool alive = true;
	int start_note = 1;

	Ant() : Way() {}
	Ant(const vector<int>& way, int len) : Way(way, len) {}
	Ant(int start_note, int len) : start_note(start_note), Way(len) {}

	int last() {
	
		if (size() <= 0)
			return start_note;

		return Way::last();
	
	}


	bool passed_through_point(int where_to, int where_from) {

		for (int w = 0; w < size()-1; w++) {

			if (way[w] == where_from && way[w + 1] == where_to)
				return true;
					
		}

		/*if (last() == where_to && penultimate() != -1 && penultimate() == where_from)
			return true;*/
		
		return false;
	
	}

	vector<int> getAvailableWays(vector<int> nodes) {
		if (nodes.size() <= 0)
			return nodes;

		if (way.size() <= 0)
			return nodes;

		vector<int> answer = vector<int>();


		for (int n = 0; n < nodes.size(); n++) {
			bool flag = false;

			for (int w = 0; w < way.size(); w++) {
				if (nodes[n] == way[w]) {
					flag = true;
					break;
				}
			
			}

			if (!flag)
				answer.push_back(nodes[n]);

		}

		return answer;
	}

	int choosingWay(vector<double> weights_ways, double sum_weight) {

		if (weights_ways.size() <= 0 || sum_weight == 0)
			return 0;

		if (weights_ways.size() == 1) {
			return 0;
		}
		
		// Создаем генератор случайных чисел
		random_device rd;  // Источник случайности (аппаратный или псевдослучайный)
		mt19937 gen(rd()); // Генератор случайных чисел (Mersenne Twister)

		// Определяем распределение от 0 до 1
		uniform_real_distribution<> dist(0.0, 1.0);

		double random_number = dist(gen);

		double normalized_chance;
		double accumulate_probability = 0;

		int selected_way;
		/*cout << "sum: " << sum_weight << " | " << weights_ways << '\n';*/
		
		

		for (int w = 0; w < weights_ways.size(); w++) {

			normalized_chance = weights_ways[w] / sum_weight;

			if (weights_ways[w] == start_note)
				continue;

			
		/*	cout << "weights_ways[w]: " << weights_ways[w] << '\n';
			cout << "normalized_chance: " << normalized_chance << '\n';*/
			accumulate_probability += normalized_chance;
			/*cout << "accumulate_probability: " << accumulate_probability << '\n';
			cout << "random_number: " << random_number << '\n';
			cout << "w: " << w << '\n';*/


			if (random_number <= accumulate_probability) {
				return w;
			}

		}
	
	}


	int choosingWay(pair<vector<double>, double> result_weight_ways) {

		return choosingWay(result_weight_ways.first, result_weight_ways.second);

	}

};




#endif