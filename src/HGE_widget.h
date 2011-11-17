#ifndef _HGE_WIDGET_H_
#define _HGE_WIDGET_H_

typedef struct HGE_widget_t HGE_widget_t;
struct HGE_widget_t
{
  void (*focus)(HGE_widget_t*,int);
  void (*key)(HGE_widget_t*,int);
  void (*click)(HGE_widget_t*,int);
};

#endif
