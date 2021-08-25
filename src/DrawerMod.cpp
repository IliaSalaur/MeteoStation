#include <DrawerMod.h>

DrawerMod::DrawerMod(GxEPD_Class * display, int _w, int _h)
{
    _display = display;
    _display->setRotation(1);
    _lastName = "";
    w = _w;
    h = _h;
}

void DrawerMod::begin()
{
    _display->init();
    _display->setRotation(1);
    _display->eraseDisplay();
}

void DrawerMod::drawContext(DrawContext context)
{
    DEBUG(context.elements.size());
    _display->fillScreen(GxEPD_WHITE);
    for(size_t i = 0; i < context.elements.size(); i++)
    {
        _drawEl(context.elements[i].el);
    }
    if(_lastName == context.name)
    {
        this->_update(0, 0, w, h);    
    } 
    else this->_update();
    _lastName = context.name;
}

void DrawerMod::_drawText(int x, int y, int w, int h, String t, GFXfont* f, uint32_t col = 0x0)
{
    _display->setTextColor(col);
    _display->setFont(f);
    _display->fillRect(x, y-h, w, h, GxEPD_WHITE);
    _display->setCursor(x, y);
    _display->print(t);
}

void DrawerMod::_drawRect(int x, int y, int w, int h, bool fill, uint32_t col = 0x0)
{
    if(fill) _display->fillRect(x, y, w, h, col);
    else _display->drawRect(x, y, w, h, col);
}

void DrawerMod::_drawLine(int x0, int y0, int x1, int y1, uint32_t col = 0x0)
{
    _display->drawLine(x0, y0, x1, y1, col);
}

void DrawerMod::_drawBitmap(int x, int y, int w, int h, const unsigned char* b, uint32_t col = 0x0)
{
    _display->fillRect(x, y, w, h, GxEPD_WHITE);
    _display->drawBitmap(b, x, y, w, h, col);
}

void DrawerMod::_drawEl(IElement *el)
{
    //DEBUG("Drawing el: ");
    switch(el->eltype)
    {
    case ElType::LINE:DEBUG("Line")
        this->_drawLine(el->x, el->y, el->width, el->height, el->col);
        break;

    case ElType::RECT:DEBUG("Rect"){
        ElRect* rect = (ElRect*) el;
        this->_drawRect(el->x, el->y, el->width, el->height, rect->fill, el->col);
        }
        break;

    case ElType::BITMAP:{DEBUG("Bitmap")
        ElBitmap* elbm = (ElBitmap*) el;// element bitmap
        this->_drawBitmap(elbm->x, elbm->y, elbm->width, elbm->height, elbm->bitmap, elbm->col);
        }
        break;

    case ElType::TEXT:{DEBUG("Text")
        ElText* elt = (ElText*) el; // element text
        //ElText* elt = static_cast<ElText*>(el);
        DEBUG(elt->text)
        this->_drawText(elt->x, elt->y, elt->width, elt->height, elt->text, elt->font, elt->col);
        }
        break;

    case ElType::CHART:{DEBUG("Chart")
        ElChart* chart = (ElChart*) el;
        this->_drawChart(chart);
        }
        break;

    default:
    DEBUG("Err:");
    DEBUG(int(el->eltype));
        break;
    }
}

bool DrawerMod::addPage(DrawContext page)
{
    for(auto p : _pages)
    {
        if (p.name == page.name)
        {
            return 0;
        }
    }
    _pages.push_back(page);
    return 1;
}

void DrawerMod::drawPage(String name)
{
    for(auto p : _pages)
    {
        if (p.name == name)
        {
            this->drawContext(p);
            return;
        }
    }
}

void DrawerMod::drawNextPage()
{
    for(size_t i = 0; i < _pages.size(); i++)
    {
        if (_pages[i].name == _lastName)
        {
            if(i + 1 < _pages.size())
            {
                this->drawContext(_pages[i + 1]);
            }
            else
            {
                this->drawContext(_pages[0]);
            }
            return;
        }
    }
}

void DrawerMod::redrawLastPage()
{
    this->drawPage(_lastName);
}

void DrawerMod::_drawChart(ElChart* chart)
{
    this->_drawBitmap(0, 0, w, h, chart->bitmap);
    int lastCoords[2] = {-1, -1};
    for (uint8_t i = 0; i < 24; i++)
    {
        if(chart->average->a[i] != -1.0F)
        {
            //uint8_t x = chart->x;
            //x = x + i * chart->width;
            uint8_t x = map(i, 0, 23, chart->x, chart->width); //0, 23 - hours (from 0 to 23)

            //uint8_t y = chart->y;
            //y = y - round(chart->average[i]) * chart->height;
            uint8_t y = map(chart->average->a[i], chart->minValue, chart->maxValue, 0, chart->y);

            _display->drawRect(x - 1, y - 1, 3, 3, GxEPD_BLACK);
            if (lastCoords[0] != -1 && lastCoords[1] != -1)
                {
                    _display->drawLine(x - 1, y, lastCoords[0] + 1, lastCoords[1], GxEPD_BLACK);
                }
            lastCoords[0] = x;
            lastCoords[1] = y;
            DEBUG(String("Data: ") + String(chart->average->a[i]) + String(";  hour: ") + String(i) + String(";  x: ") + String(x) + String(";  y: ") + String(y) + String(";"));
        }
        else DEBUG("Bad data:" + String(chart->average->a[i]) + String(";  hour: ") + String(i))
    }
    DEBUG(chart->x)
    DEBUG(chart->width)
}

void DrawerMod::_update(int x, int y, int width, int height)
{
    _display->updateWindow(x, y, width, height, true);
}

void DrawerMod::_update()
{
    _display->update();
}

DrawerMod::~DrawerMod(){}
