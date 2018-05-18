#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sstream>

class VertexOfEdgeOutOfGraph : public std::exception
{
    public:
        template <typename T>
        VertexOfEdgeOutOfGraph(const T& vertex, const std::tuple<T, T, int>& edge) : std::exception()
        {
            std::ostringstream vStr;
            std::ostringstream eStr;
            vStr << vertex;
            eStr << std::get<0>(edge) <<"-"<<std::get<1>(edge)<<" "<<std::get<2>(edge);
            message = std::string("Graph does not consist vertex ") + vStr.str() + std::string(" of edge ") + eStr.str();
        }

        virtual const char* what() const noexcept override
        {
            return message.c_str();
        }
    private:
        std::string message;

};

template <typename T>
class Graph
{
    public:
        Graph(const std::vector<std::pair<T, int>>& vertices);  //name and weight
        void addEdge(const std::tuple<T,T,int>& edge);
        void addEdge(std::tuple<T,T,int>&& edge);
        const std::vector<std::vector<std::pair<int,int>>>& getMarkedAdjacentList()const;
        const std::unordered_map<T, int>& getMarking()const;
        const std::vector<int>& getMarkedDurations()const;
        const std::vector<std::pair<T, int>>& getReverseMarking()const;
    private:
        std::vector<std::pair<T, int>> m_vertices;
        std::vector<std::vector<std::pair<int, int>>> m_markedAdjList;
        std::unordered_map<T, int> m_marking;
        std::vector<int> m_markedDurations;
};

template<typename T>
Graph<T>::Graph(const std::vector<std::pair<T,int>>& vertices)
{
    m_vertices = vertices;
    auto last = std::unique(m_vertices.begin(), m_vertices.end(),
                            [&](const auto& a, const auto& b)->bool{return a.first == b.first;});
    m_vertices.erase(last, m_vertices.end());
    m_markedAdjList = std::vector<std::vector<std::pair<int,int>>>(m_vertices.size());
    m_markedDurations = std::vector<int>(m_vertices.size());
    auto markNumber = 0;
    for (auto vertex : m_vertices){
        m_marking[vertex.first] = markNumber;
        m_markedDurations[markNumber] = vertex.second;
        ++markNumber;
    }
}

template<typename T>
void Graph<T>::addEdge(const std::tuple<T,T,int> &edge)
{
    const auto& findFirst = m_marking.find(std::get<0>(edge));
    const auto& findSecond = m_marking.find(std::get<1>(edge));
    if (findFirst == m_marking.end()) {
        throw VertexOfEdgeOutOfGraph(std::get<0>(edge), edge);
    }
    if (findSecond == m_marking.end()){
        throw VertexOfEdgeOutOfGraph(std::get<1>(edge), edge);
    }
    std::make_pair(findSecond->second, std::get<2>(edge));
    m_markedAdjList[findFirst->second].push_back(std::make_pair(findSecond->second, std::get<2>(edge)));
}

template<typename T>
void Graph<T>::addEdge(std::tuple<T,T,int>&& edge)
{
    const auto& findFirst = m_marking.find(std::get<0>(edge));
    const auto& findSecond = m_marking.find(std::get<1>(edge));
    if (findFirst == m_marking.end()) {
        throw VertexOfEdgeOutOfGraph(std::get<0>(edge), edge);
    }
    if (findSecond == m_marking.end()){
        throw VertexOfEdgeOutOfGraph(std::get<1>(edge), edge);
    }
    std::make_pair(findSecond->second, std::get<2>(edge));
    m_markedAdjList[findFirst->second].push_back(std::make_pair(findSecond->second, std::get<2>(edge)));
}

template<typename T>
const std::vector<std::vector<std::pair<int,int>>>& Graph<T>::getMarkedAdjacentList()const
{
    return m_markedAdjList;
}

template<typename T>
const std::unordered_map<T, int>& Graph<T>::getMarking()const
{
    return m_marking;
}

template <typename T>
const std::vector<int>& Graph<T>::getMarkedDurations()const
{
    return m_markedDurations;
}

template<typename T>
const std::vector<std::pair<T, int> >& Graph<T>::getReverseMarking() const
{
    return m_vertices;
}

#endif // GRAPH_H
