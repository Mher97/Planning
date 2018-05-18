#ifndef SHCEDULER_H
#define SHCEDULER_H

#include <string.h>
#include "../Engine/Graph.h"

class DAGCheckFailed : public std::exception
{
    public:
        template <typename T>
        DAGCheckFailed(T&& errorText) : std::exception(), m_errorText(std::forward<T>(errorText))
        {

        }
        virtual const char* what() const noexcept override
        {
            std::string stdWhat = message + m_errorText;
            char *result = new char[stdWhat.length() + 1]();
            memcpy(result, stdWhat.c_str(), stdWhat.length());
            return result;
        }
    private:
        static const std::string message;
        std::string m_errorText;
};

class TaskInfo
{
    public:
        TaskInfo(std::string taskId = {}, int start = 0, int finish = 0) :
            TaskId(taskId), StartTime(start), FinishTime(finish){}
        std::string TaskId;
        int StartTime;
        int FinishTime;
};

class Shceduler
{
    public:
        Shceduler();
        const std::vector<std::vector<const TaskInfo*>>
                scheduleData(const std::unordered_map<std::string, std::pair<int, std::vector<std::string>>>& data);
    private:
        void topologicalSortWithDAGCheck();
        void topologicalSortWithDAGCheckHelper(int vertex = 0);
        void findLongestPath();
        void calculateResult();
        std::vector<std::vector<std::pair<int,int>>> m_adjList;
        std::unordered_map<std::string, int> m_marking;
        std::vector<int> m_markedDurations;
        std::vector<std::pair<std::string, int>> m_reverseMarking;
        std::vector<int> m_preOrder;
        std::vector<int> m_postOrder;
        std::vector<int> m_topology;
        std::vector<int> m_maxWeight;
        std::vector<std::pair<int,int>> m_csEdge;
        std::vector<int> m_inLongestPath;
        std::vector<std::pair<int,int>> m_planning;
        std::vector<std::vector<const TaskInfo*>> m_result;
        void resetPreviousCalculations();
};

#endif // SHCEDULER_H
