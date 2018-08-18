#ifndef ITEMTYPE_H
#define ITEMTYPE_H

#include <QString>

namespace ItemType
{
    enum class Type : int
    {
        Default = 0,
        RootItem = 1,
        WorkSpaceItem = 2,
        ProjectItem = 3,
        BranchItem = 4,
        MonitorItem = 5,
        MappingItem = 6,
        PlanningItem = 7
    };

    static QString ItemTypetoString(Type type)
    {
        switch (type)
        {
            case Type::Default:
                return "Default";
            case Type::RootItem:
                return "Root Item";
            case Type::WorkSpaceItem:
                return "Workspace";
            case Type::ProjectItem:
                return "Project";
            case Type::BranchItem:
                return "Branch";
            case Type::MonitorItem:
                return "Monitoring";
            case Type::MappingItem:
                return "Mapping";
            case Type::PlanningItem:
                return "Planning";
            default:
                return {};
        }
    }
}

#endif // ITEMTYPE_H
