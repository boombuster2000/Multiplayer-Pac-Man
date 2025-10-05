#pragma once

#include "Enums.h"
#include "raylib.h"

namespace Components {


    class RenderableObject
	{

    private:
    Vector2 m_drawPoint;
    Vector2 m_anchorPointPosition;
    AnchorPoint m_anchorPoint;
    bool m_visible;
    
    protected:
    void UpdateDrawPoint();
    
    public:
    
    RenderableObject(Vector2 anchorPointPosition, bool visible = true, AnchorPoint anchorPoint = TOP_LEFT);
    virtual ~RenderableObject() = default;
    
    virtual void SetAnchorPointPosition(Vector2 position); // Sets anchor point position
    //virtual Vector2 GetPosition() const; // Returns top left position
    virtual Vector2 GetPosition(AnchorPoint anchorPoint = TOP_LEFT) const;

    virtual void SetAnchorPoint(AnchorPoint anchorPoint);
    virtual AnchorPoint GetAnchorPoint() const;
    
    virtual void SetVisibility(bool visible);
    virtual bool IsVisible() const;
    
    virtual Vector2 GetSize() const = 0;
    virtual void Render() const = 0;
        
	};

}