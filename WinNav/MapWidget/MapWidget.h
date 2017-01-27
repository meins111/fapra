#ifndef _MAPWIDGET_H_
#define _MAPWIDGET_H_

#include <marble/MarbleWidget.h>
#include "../GraphLayer/GraphLayer.h"

/**
 * @brief 
 */
class MapWidget : public Marble::MarbleWidget {
	GraphLayer graphLayer;
public:
	MapWidget(QWidget *widget);
	GraphLayer &getGraphLayer();
};

#endif
