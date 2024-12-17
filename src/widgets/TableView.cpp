#include "tp_maps_ui/widgets/TableView.h"

#include "tp_utils/RefCount.h"
#include "tp_utils/TimeUtils.h"

namespace tp_maps_ui
{

namespace
{
struct ItemDetails_lt
{
  size_t row{0};
  Widget* widget{nullptr};
  bool update{true};
};
}

//##################################################################################################
struct TableView::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::TableView::Private");
  TP_NONCOPYABLE(Private);

  Q* q;

  std::function<size_t()> numberOfRows;
  std::function<Widget*(size_t, Widget*)> widgetForRow;

  std::vector<ItemDetails_lt> items;

  Orientation orientation;

  size_t firstVisibleRow{0};
  size_t numberOfVisibleRows{0};

  //The position in the list
  float offset{0.0f};
  float rowHeight{32.0f};
  float previousAvailableSpace{0.0f};

  bool updateVisibility{false};


  //################################################################################################
  Private(Q* q_, Orientation orientation_):
    q(q_),
    orientation(orientation_)
  {

  }

  //################################################################################################
  void calculateVisibleRoes(float availableSpace)
  {
    if(std::fabs(previousAvailableSpace-availableSpace)>0.01f)
    {
      updateVisibility = true;
      previousAvailableSpace = availableSpace;
    }

    if(!updateVisibility)
      return;

    updateVisibility = false;

    firstVisibleRow     = size_t(std::floor(offset/rowHeight));
    numberOfVisibleRows = size_t(std::ceil(availableSpace/rowHeight))+1;

    size_t c=count();
    size_t max=0;
    if(c>firstVisibleRow)
      max=c-firstVisibleRow;

    if(numberOfVisibleRows>max)
      numberOfVisibleRows = max;
  }

  //################################################################################################
  size_t count()
  {
    return numberOfRows?numberOfRows():0;
  }
};

//##################################################################################################
TableView::TableView(Orientation orientation, Widget* parent):
    Widget(parent),
    d(new Private(this, orientation))
{

}

//##################################################################################################
TableView::~TableView()
{
  delete d;
}

//##################################################################################################
void TableView::setGeometry(float x, float y, float w, float h)
{
  Widget::setGeometry(x, y, w, h);
  dataChanged();
}

//##################################################################################################
void TableView::dataChanged()
{
  for(auto& item : d->items)
    item.update = true;
  d->updateVisibility = true;

  update();
}

//##################################################################################################
void TableView::setNumberOfRowsCallback(const std::function<size_t()>& numberOfRows)
{
  d->numberOfRows = numberOfRows;
  d->updateVisibility = true;
  dataChanged();
}

//##################################################################################################
void TableView::setWidgetForRowCallback(const std::function<Widget*(size_t, Widget*)>& widgetForRow)
{
  d->widgetForRow = widgetForRow;
  d->updateVisibility = true;
  dataChanged();
}

//##################################################################################################
void TableView::render(tp_maps::RenderInfo& renderInfo)
{
  TP_FUNCTION_TIME("TableView::render");

  Widget::render(renderInfo);

  d->calculateVisibleRoes(height());

  if(d->items.size()<d->numberOfVisibleRows)
    d->items.resize(d->numberOfVisibleRows);

  for(size_t r=0; r<d->numberOfVisibleRows; r++)
  {
    size_t row = d->firstVisibleRow + r;
    auto& item = d->items[r];

    if(item.row != row || item.update || !item.widget)
    {
      item.row = row;
      item.update = false;
      if(d->widgetForRow)
      {
        Widget* itemWidget = d->widgetForRow(row, item.widget);
        if(itemWidget != item.widget)
        {
          delete item.widget;
          item.widget = itemWidget;

          if(itemWidget)
            addWidget(itemWidget);
        }

        if(itemWidget)
          itemWidget->setGeometry(0.0f, d->offset+(float(r)*d->rowHeight), width(), d->rowHeight);
      }
    }
  }

  for(size_t r=d->numberOfVisibleRows; r<d->items.size(); r++)
    d->items.at(r).widget->setVisible(false);
}

//##################################################################################################
void TableView::invalidateBuffers()
{
  Widget::invalidateBuffers();
}

//##################################################################################################
bool TableView::mouseEvent(const tp_maps::MouseEvent& event)
{
  return Widget::mouseEvent(event);
}

//##################################################################################################
void TableView::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
