#include "Shceduler.h"

const std::string DAGCheckFailed::message = "Cycle Founded.";

Shceduler::Shceduler()
{

}

const std::vector<std::vector<const TaskInfo*>>  Shceduler::scheduleData(const std::unordered_map<std::string, std::pair<int, std::vector<std::string> > > &data)
{
    resetPreviousCalculations();
    std::vector<std::pair<std::string,int>> vertices;  //name and weight of vertex
    for (const auto& it : data){
        vertices.push_back(std::make_pair(it.first, it.second.first));
    }
    Graph<std::string> graph(vertices);
    try{
        for (const auto& it : data){
            for (const auto& dest : it.second.second){
                graph.addEdge(std::make_tuple(it.first, dest, it.second.first));
            }
        }
        m_adjList = graph.getMarkedAdjacentList();
        m_markedDurations = graph.getMarkedDurations();
        m_marking = graph.getMarking();
        m_reverseMarking = graph.getReverseMarking();
        m_preOrder = std::vector<int>(m_adjList.size());
        m_postOrder = std::vector<int>(m_adjList.size());
        m_maxWeight = std::vector<int>(m_adjList.size());
        m_csEdge = std::vector<std::pair<int,int>>(m_adjList.size());
        m_inLongestPath = std::vector<int>(m_adjList.size());
        m_planning = std::vector<std::pair<int,int>>(m_adjList.size());
        topologicalSortWithDAGCheck();
        findLongestPath();
        calculateResult();
    }catch(std::exception&){
        throw;
    }
    return m_result;
}

void Shceduler::topologicalSortWithDAGCheck()
{
    for (size_t i = 0; i < m_preOrder.size(); ++i){
        if (!m_preOrder[i])
            topologicalSortWithDAGCheckHelper(i);
    }
}

void Shceduler::topologicalSortWithDAGCheckHelper(int vertex)
{
    m_preOrder[vertex] = 1;
    for (const auto& to : m_adjList[vertex])
    {
        if (!m_preOrder[to.first]){
            topologicalSortWithDAGCheckHelper(to.first);
        }
        else{
            if (!m_postOrder[to.first]){
                throw DAGCheckFailed(std::string(m_reverseMarking[vertex].first + " to "
                                                 + m_reverseMarking[to.first].first));
            }
        }
    }
    m_postOrder[vertex] = true;
    m_topology.push_back(vertex);
}

void Shceduler::findLongestPath()
{
    std::reverse(m_topology.begin(), m_topology.end());
    for (const auto& vertex : m_topology){
        for (size_t i = 0; i < m_adjList[vertex].size(); ++i){
            int s = vertex;
            int t = m_adjList[vertex][i].first;
            int w = m_adjList[vertex][i].second;
            if (m_maxWeight[s] + w > m_maxWeight[t]){
                m_maxWeight[t] = m_maxWeight[s] + w;
                m_csEdge[t] = std::make_pair(s, t);
            }
        }
    }
}

void Shceduler::calculateResult()
{
    if (m_maxWeight.empty())
        return;
    int maxPath = m_maxWeight[0];
    int vertex = 0;
    for (size_t i = 1; i < m_maxWeight.size(); ++i){
        if (m_maxWeight[i] > maxPath){
            maxPath = m_maxWeight[i];
            vertex = i;
        }
    }
    int tempVertex = vertex;
    for (;;)
    {
        if (maxPath == 0)
            break;
        m_inLongestPath[m_csEdge[vertex].first] = 1;
        m_planning[m_csEdge[vertex].first] = std::make_pair(m_maxWeight[m_csEdge[vertex].first] + 1 ,
                                                            m_maxWeight[m_csEdge[vertex].second]);
        maxPath -= (m_maxWeight[m_csEdge[vertex].second] - m_maxWeight[m_csEdge[vertex].first]);
        vertex = m_csEdge[vertex].first;
     }
     m_inLongestPath[tempVertex] = 0;
     for (size_t i = 0; i < m_inLongestPath.size(); ++i) {
        if (m_inLongestPath[i] != 1){
            m_planning[i] = std::make_pair(m_maxWeight[i] + 1 ,m_maxWeight[i] + m_markedDurations[i]);
        }
     }
     std::vector<const TaskInfo*> sortPlaning;
     for (const auto& it : m_marking){
         sortPlaning.push_back(new TaskInfo(it.first, m_planning[it.second].first, m_planning[it.second].second));
     }
     std::sort(sortPlaning.begin(), sortPlaning.end(),
               [&](const auto &firstInfo, const auto& secondInfo)->bool
                  {return firstInfo->StartTime < secondInfo->StartTime;});
     std::vector<const TaskInfo*> phaseVector;
     const TaskInfo* previous = new TaskInfo();
     for (const auto it : sortPlaning){
         if (it->StartTime != previous->StartTime){
             if (!phaseVector.empty()){
                 m_result.push_back(phaseVector);
                 phaseVector.clear();
             }
         }
         phaseVector.push_back(it);
         previous = it;
     }
     m_result.push_back(phaseVector);
}

void Shceduler::resetPreviousCalculations()
{
    for(auto vector : m_result){
        for (auto it : vector){
            delete it;
            it = nullptr;
        }
        vector.clear();
    }
    m_result.clear();
    m_preOrder.clear();
    m_postOrder.clear();
    m_topology.clear();
    m_maxWeight.clear();
    m_csEdge.clear();
    m_inLongestPath.clear();
    m_planning.clear();
}


