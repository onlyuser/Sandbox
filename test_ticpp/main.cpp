#include <iostream>
#include <map>

#define TIXML_USE_TICPP
#include <ticpp/ticpp.h>

void visit(ticpp::Node* node, size_t depth);

int main(int argc, char** argv)
{
    if(argc != 2)
        return -1;
    char* pcFilename = argv[1];
    try
    {
        ticpp::Document doc(pcFilename);
        doc.LoadFile();
        visit(&doc, 0);
    }
    catch(ticpp::Exception& ex)
    {
        std::cout << ex.what();
    }
    return 0;
}

void visit(ticpp::Node* node, size_t depth)
{
    if(dynamic_cast<ticpp::Document*>(node))
    {
        if(!node->NoChildren())
        {
            ticpp::Iterator<ticpp::Node> child;
            for(child = child.begin(node); child != child.end(); child++)
                visit(child.Get(), depth);
        }
        return;
    }
    if(dynamic_cast<ticpp::Declaration*>(node))
        return;
    std::string BlockName;
    node->GetValue(&BlockName);
    std::cout << std::string(depth*4, ' ') << BlockName << " {" << std::endl;
    depth++;
    ticpp::Element* elem = dynamic_cast<ticpp::Element*>(node);
    if(elem)
    {
        std::map<std::string, std::string> attrib_map; // in case you need it
        ticpp::Iterator< ticpp::Attribute > attribute;
        for(attribute = attribute.begin(elem); attribute != attribute.end(); attribute++)
        {
            std::string Key, Value;
            attribute->GetName(&Key);
            attribute->GetValue(&Value);
            attrib_map[Key] = Value;
            std::cout << std::string(depth*4, ' ') << Key << ": " << Value << std::endl;
        }
    }
    if(!node->NoChildren())
    {
        ticpp::Iterator<ticpp::Node> child;
        for(child = child.begin(node); child != child.end(); child++)
            visit(child.Get(), depth);
    }
    depth--;
    std::cout << std::string(depth*4, ' ') << "}" << std::endl;
}
