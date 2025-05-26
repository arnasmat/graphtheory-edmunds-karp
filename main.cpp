#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>



struct Edge {
    int to{}, weight{};
};

void modify_graph(std::pmr::unordered_map<int, std::vector<Edge>> &Graph, const int s, const int t,
                 std::pmr::unordered_map<int, int> &width, std::pmr::unordered_map<int, int> &previous);

// pq
struct CompareWeight {
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.first < b.first; // Max heap by weight
    }
};

//Input is in the following format: from to weight
//In the example input, the 0 is the source and 5 is the t
void read_graph(std::pmr::unordered_map<int, std::vector<Edge>> &Graph , const std::string &filename="../input.txt") {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error opening file " << filename << std::endl;
        throw std::runtime_error("Error opening file");
    }

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        int from, to, weight;
        ss >> from >> to >> weight;
        Graph[from].push_back({to, weight});
    }
}

void iterate_graph(std::pmr::unordered_map<int, std::vector<Edge>> &Graph) {
    for (const auto &i: Graph) {
        std::cout << i.first << " -> ";
        for (auto j: i.second) {
            std::cout << j.to << " w" << j.weight << ";";
        }
        std::cout << std::endl;
    }
}


void max_flow_path(std::pmr::unordered_map<int, std::vector<Edge>> &Graph, int s, int t) {

    std::pmr::unordered_map<int, std::vector<Edge> > GraphCopy = Graph;

    int bendras_srautas{0};

    while (true) {
        std::pmr::unordered_map<int, int> width; //<virsune, jos width>
        std::pmr::unordered_map<int, int> previous; //<virsune, jos prev>

        for (int i = 0; i < GraphCopy.size(); i++) {
            width[i] = INT_MIN;
            previous[i] = -1;
        }

        width[s] = INT_MAX;

        std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, CompareWeight> pq;
        for (int i = 0; i < GraphCopy.size(); i++) {
            pq.push({width[i], i});
        }


        while (!pq.empty()) {
            auto [wdt, u] = pq.top();
            pq.pop();

            if (width[u] == INT_MIN) break;

            for (auto &v : GraphCopy[u]) {
                int temp = std::max(width[v.to], std::min(width[u], v.weight));
                if (temp > width[v.to]) {
                    width[v.to] = temp;
                    previous[v.to] = u;
                    pq.push({temp, v.to});
                }
            }
        }


        int ind = t;
        if (width[t] < 1) {
            break;
        }
        while (ind != s) {
            std::cout <<"Virsune: "<< ind << "; Srautas:  "<<width[ind]<< " " <<"Praeita virsune: "<< previous[ind]<<"\n";
            ind = previous[ind];
        }
        std::cout << "\n\n";
        bendras_srautas += width[t];

        modify_graph(GraphCopy, s, t, width, previous);
    }
    std::cout<<"Bendras srautas: "<<bendras_srautas<<"\n";
}

void modify_graph(std::pmr::unordered_map<int, std::vector<Edge>> &Graph, const int s, const int t,
                 std::pmr::unordered_map<int, int> &width, std::pmr::unordered_map<int, int> &previous) {

    int bottleneck = width[t];
    int ind = t;

    while (ind != s) {

        // bsk neefektyvu cia, gal reikejo grafa daryt kaip mapa, bet ajjj tingiu perdaryt, veikia vis tiek!
        for (auto& v : Graph[previous[ind]]) {
            if (v.to == ind) {
                v.weight -= bottleneck;
                break;
            }
        }

        //pakeist svori pagal bottlenecka, jei nera briaunos i priesinga puse, prideti ja
        //velgi bsk neefektyvu kodo prasme bet ajj
        bool opposite_edge = false;
        for (auto& v : Graph[ind]) {
            if (v.to == previous[ind]) {
                v.weight += bottleneck;
                opposite_edge = true;
                break;
            }
        }

        if (!opposite_edge) {
            Graph[ind].push_back({previous[ind], bottleneck});
        }

        ind = previous[ind];
    }
}



int main() {
    //Here the graph is defined as an unordered map of node name (int) and Adjacency list (vector<Edge>)
    std::pmr::unordered_map<int, std::vector<Edge>> Graph;
    int s{0};
    int t{5};
    read_graph(Graph, "../i2.txt");
    iterate_graph(Graph);
    std::cout<<"-------------------------------\n";
    max_flow_path(Graph, s, t);



}
