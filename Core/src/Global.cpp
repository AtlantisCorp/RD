//
//  Global.cpp
//  RD
//
//  Created by Jacques Tronconi on 21/07/2018.
//

#include "Global.h"

namespace RD
{
    /////////////////////////////////////////////////////////////////////////////////
    const std::vector<std::string> explode(const std::string& s, const char& c)
    {
        std::string buff{""};
        std::vector<std::string> v;
        
        for(auto n:s)
        {
            if(n != c) buff+=n; else
            if(n == c && buff != "") { v.push_back(buff); buff = ""; }
        }
        if(buff != "") v.push_back(buff);
        
        return v;
    }
}
    
/////////////////////////////////////////////////////////////////////////////////
std::ostream& operator << (std::ostream& stream, const RD::Version& version)
{
    stream << version.major << "." << version.minor << "." << version.patch << ":" << version.build;
    return stream;
}

/////////////////////////////////////////////////////////////////////////////////
std::istream& operator >> (std::istream& stream, RD::Version& version)
{
    stream >> version.major;
    if (stream.get() == '.')
        stream >> version.minor;
    if (stream.get() == '.')
        stream >> version.patch;
    if (stream.get() == ':')
        stream >> version.build;
    
    return stream;
}
