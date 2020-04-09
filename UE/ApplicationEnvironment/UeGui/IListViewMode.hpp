#pragma once

#include "IUeGui.hpp"
#include "BaseMode.hpp"
#include <utility>

namespace ue
{

class IUeGui::IListViewMode : public IUeGui::BaseMode
{
public:
    using Selection = unsigned;
    using OptionalSelection = std::pair<bool,Selection>;

    virtual ~IListViewMode() = default;

    virtual OptionalSelection getCurrentItemIndex() const = 0;
    virtual void addSelectionListItem(const std::string& label, const std::string& tooltip) = 0;
    virtual void clearSelectionList() = 0;
};

}
