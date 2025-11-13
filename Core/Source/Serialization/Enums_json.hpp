#pragma once
#include <nlohmann/json.hpp>
#include "UIComponents/Enums.h"

namespace UIComponents {

    using nlohmann::json;

    // ----------------- AnchorPoint -----------------
    inline void to_json(json& j, const AnchorPoint& a)
    {
        switch (a)
        {
        case AnchorPoint::TOP_LEFT:      
            j = "TOP_LEFT"; 
            break;
        case AnchorPoint::TOP_MIDDLE:    
            j = "TOP_MIDDLE"; 
            break;
        case AnchorPoint::TOP_RIGHT:     
            j = "TOP_RIGHT"; 
            break;
        case AnchorPoint::MIDDLE_LEFT:   
            j = "MIDDLE_LEFT"; 
            break;
        case AnchorPoint::MIDDLE:        
            j = "MIDDLE"; 
            break;
        case AnchorPoint::MIDDLE_RIGHT:  
            j = "MIDDLE_RIGHT"; 
            break;
        case AnchorPoint::BOTTOM_LEFT:   
            j = "BOTTOM_LEFT"; 
            break;
        case AnchorPoint::BOTTOM_MIDDLE: 
            j = "BOTTOM_MIDDLE"; 
            break;
        case AnchorPoint::BOTTOM_RIGHT:  
            j = "BOTTOM_RIGHT"; 
            break;
        }
    }

    inline void from_json(const json& j, AnchorPoint& a)
    {
        const auto& s = j.get<std::string>();
        if (s == "TOP_LEFT")       
            a = AnchorPoint::TOP_LEFT;

        else if (s == "TOP_MIDDLE")     
            a = AnchorPoint::TOP_MIDDLE;

        else if (s == "TOP_RIGHT")      
            a = AnchorPoint::TOP_RIGHT;

        else if (s == "MIDDLE_LEFT")    
            a = AnchorPoint::MIDDLE_LEFT;

        else if (s == "MIDDLE")         
            a = AnchorPoint::MIDDLE;

        else if (s == "MIDDLE_RIGHT")   
            a = AnchorPoint::MIDDLE_RIGHT;

        else if (s == "BOTTOM_LEFT")    
            a = AnchorPoint::BOTTOM_LEFT;

        else if (s == "BOTTOM_MIDDLE")  
            a = AnchorPoint::BOTTOM_MIDDLE;

        else if (s == "BOTTOM_RIGHT")   
            a = AnchorPoint::BOTTOM_RIGHT;

        else 
            throw std::invalid_argument("Invalid AnchorPoint: " + s);
    }

    // ----------------- Alignment -----------------
    inline void to_json(json& j, const Alignment& a)
    {
        switch (a)
        {
        case Alignment::LEFT:   
            j = "LEFT"; 
            break;
        case Alignment::CENTER: 
            j = "CENTER"; 
            break;
        case Alignment::RIGHT:  
            j = "RIGHT"; 
            break;
        }
    }

    inline void from_json(const json& j, Alignment& a)
    {
        const auto& s = j.get<std::string>();

        if (s == "LEFT")   
            a = Alignment::LEFT;

        else if (s == "CENTER") 
            a = Alignment::CENTER;

        else if (s == "RIGHT")  
            a = Alignment::RIGHT;

        else 
            throw std::invalid_argument("Invalid Alignment: " + s);
    }

    // ----------------- Direction -----------------
    inline void to_json(json& j, const Direction& d)
    {
        switch (d)
        {
        case Direction::UP:
            j = "UP"; 
            break;
        case Direction::DOWN:  
            j = "DOWN"; 
            break;
        case Direction::LEFT:  
            j = "LEFT"; 
            break;
        case Direction::RIGHT: 
            j = "RIGHT"; 
            break;
        }
    }

    inline void from_json(const json& j, Direction& d)
    {
        const auto& s = j.get<std::string>();
        
        if (s == "UP")    
            d = Direction::UP;

        else if (s == "DOWN") 
            d = Direction::DOWN;

        else if (s == "LEFT")  
            d = Direction::LEFT;

        else if (s == "RIGHT") 
            d = Direction::RIGHT;

        else 
            throw std::invalid_argument("Invalid Direction: " + s);
    }

}