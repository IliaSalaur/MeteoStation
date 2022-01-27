#ifndef DRAW_CONTEXT_H
#define DRAW_CONTEXT_H

#include <vector>
#include <Arduino.h>
#include <gfxfont.h>
#include <GxEPD.h>
#include <Average.h>

using namespace std;

enum class ElType
{
    BITMAP,
    RECT,
    LINE,
    TEXT,
    CHART,
    NULL_EL
};

struct IElement
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    ElType eltype;
    uint32_t col = 0;

    IElement(ElType elt) : eltype(elt){}

    /*
    \brief Function for setting the size and position of element
    @param _x x coordinate
    @param _y y coordinate
    @param _w width
    @param _h height
    */
    void setSize(int _x, int _y, int _w, int _h) 
    {
        x = _x;
        y = _y;
        width = _w;
        height = _h;
    }
};

struct ElBitmap : public IElement
{
    const unsigned char* bitmap;
    ElBitmap(const unsigned char* b) : bitmap(b), IElement(ElType::BITMAP){}
    ElBitmap() : IElement(ElType::BITMAP){}
};

struct ElText : public IElement
{
    String text = "text";
    GFXfont* font = nullptr;
    ElText(String t, const GFXfont* f) : text(t), font(const_cast<GFXfont*>(f)), IElement(ElType::TEXT){}
};

struct ElRect : public IElement
{
    bool fill;
    ElRect() : fill(0), IElement(ElType::RECT){}
};

struct ElLine : public IElement
{
    ElLine() : IElement(ElType::LINE){}
};

struct ElChart : public ElBitmap
{
    Average *average;
    float maxValue;
    float minValue;
    ElChart() : maxValue(0), minValue(0)
    {
        maxValue = 0;
        minValue = 0;
        eltype = ElType::CHART;
    }

    ElChart(float max, float min, Average* avg, const unsigned char* b) : maxValue(max), minValue(min), average(avg), ElBitmap(b)
    {
        maxValue = max;
        minValue = min;
        bitmap = b;
        average = avg;
        eltype = ElType::CHART;
    }
};

struct Element
{
    IElement* el;
    String label;

    Element() : el(nullptr), label(){}

    Element(IElement* e, String l) : el(e), label(l){}
};

typedef struct{vector<Element> elements; String name;} DrawContext;  

class DrawContextClass
{
private: 
    DrawContext _context;

public:
    DrawContextClass(DrawContext c)
    {
        this->setContext(c);
    }

    DrawContextClass(){}

    DrawContext getContext()
    {
        return _context;
    }
    /*
    @tparam T type of context that you want to set (ElText, ElRect and others)
    @param l label of the element you want to modify
    @return Pointer of T (for modifing the element)
    */
    template<typename T>
    auto set(String l)
    {
        for(auto e : _context.elements)
        {
            if(e.label == l)
            {
                T* t = (T*)e.el;
                return t;
            }
        }
        //return _context.elements[0].el;
    }

    void setContext(DrawContext c)
    {
        _context = c;
    }
};

class DrawContextBuilder
{
private:
    DrawContext _els;
    void _addFig(String l, int x, int y, int w, int h, IElement* fig, uint32_t col = GxEPD_BLACK)
    {
        fig->setSize(x, y, w, h);
        fig->col = col;

        _els.elements.push_back(Element(fig, l));
    }
    
public:
    DrawContextBuilder(String name)
    {
        _els.name = name;
    }

    DrawContextBuilder* addLine(String l, int x, int y, int x1, int y1, uint32_t col = GxEPD_BLACK)
    {
        IElement *fig = new ElLine;
        this->_addFig(l, x, y, x1, y1, fig, col);
        return this;
    }

    DrawContextBuilder* addRect(String l, int x, int y, int w, int h, bool fill, uint32_t col = GxEPD_BLACK)
    {
        ElRect *fig = new ElRect;
        fig->fill = fill;
        this->_addFig(l, x, y, w, h, fig, col);
        return this;
    }

    DrawContextBuilder* addBitmap(String l, int x, int y, int w, int h, const unsigned char* b, uint32_t col = GxEPD_BLACK)
    {
        IElement* bitm = new ElBitmap(b);
        bitm->setSize(x, y, w, h);
        bitm->col = col;

        _els.elements.push_back(Element(bitm, l));
        return this;
    }

    DrawContextBuilder* addText(String l, int x, int y, int w, int h, String t, const GFXfont* f, uint32_t col = GxEPD_BLACK)
    {
        IElement *text = new ElText(t, f);
        text->setSize(x, y, w, h);
        text->col = col;

        _els.elements.push_back(Element(text, l));
        return this;
    }

    /*
    \brief Function for drawing charts
    @param l label, (name of your element)
    @param init_x X of 0
    @param init_y Y of 0
    @param last_x X border of your chart
    @param last_y Y border of your chart
    @param min Min value of Average's values
    @param max Max value of Average's values
    */
    DrawContextBuilder* addChart(String l, int init_x, int init_y, int last_x, int last_y, Average *avg, float min, float max, const unsigned char* b, uint32_t col = GxEPD_BLACK)
    {
        IElement *chart = new ElChart(min, max, avg, b);
        chart->setSize(init_x, init_y, last_x, last_y);
        chart->col = col;

        _els.elements.push_back(Element(chart, l));
        return this;
    }
    
    DrawContext build()
    {
        DEBUG("Build");
        DEBUG(_els.elements.size())
        return _els;
    }
};

#endif