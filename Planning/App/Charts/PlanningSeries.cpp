#include "PlanningSeries.h"
#include "Globals.h"
#include "../Engine/Shceduler.h"

void PlanningSeries::init()
{
    updateBrush();
}

void PlanningSeries::updateBrush()
{
    setBrush(QBrush(Globals::getColorForPhase(m_phaseNumber)));
}

TaskInfo* PlanningSeries::getTaskInfo()const
{
    return m_taskInfo;
}
