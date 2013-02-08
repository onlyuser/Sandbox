#ifndef MAIN_H_
#define MAIN_H_

#include <string>
#include <vector>

class NodeIdentIface
{
public:
    typedef enum
    {
        TYPE_INNER_NODE,
        TYPE_TERM_NODE,
    } type_t;

    NodeIdentIface(type_t _type, std::string name) : m_type(_type), m_name(name)
    {}
    virtual ~NodeIdentIface()
    {}
    NodeIdentIface::type_t type() const
    {
        return m_type;
    }
    std::string &name()
    {
        return m_name;
    }

private:
    type_t m_type;
    std::string m_name;
};

struct VisitorIFace
{
    virtual ~VisitorIFace()
    {}
    virtual void dispatch_visit(NodeIdentIface* unknown) const = 0;
};

template<class T>
class Visitable
{
public:
    Visitable(T* instance) : m_instance(instance)
    {}
    virtual ~Visitable()
    {}
    virtual void accept(const VisitorIFace* v)
    {
        // "Java Tip 98" from http://en.wikipedia.org/wiki/Visitor_pattern
        v->dispatch_visit(m_instance);
    }

private:
    T *m_instance;
};

struct TermNode : public NodeIdentIface, public Visitable<TermNode>
{
    TermNode(std::string name = "")
        : NodeIdentIface(NodeIdentIface::TYPE_TERM_NODE, name), Visitable<TermNode>(this)
    {}
};

class InnerNode : public NodeIdentIface, public Visitable<InnerNode>
{
public:
    InnerNode(size_t n, std::string name = "")
        : NodeIdentIface(NodeIdentIface::TYPE_INNER_NODE, name), Visitable<InnerNode>(this)
    {
        for(int i=0; i<static_cast<int>(n); i++)
            m_children.push_back(new TermNode());
    }
    ~InnerNode()
    {
        for(auto p = m_children.begin(); p != m_children.end(); p++)
        {
            if(*p)
                delete *p;
        }
    }
    TermNode* child(size_t index)
    {
        if(index >= m_children.size())
            return NULL;
        return m_children[index];
    }
    size_t size() const
    {
        return m_children.size();
    }

private:
    std::vector<TermNode*> m_children;
};

struct VisitorDFS : public VisitorIFace
{
    virtual void visit(InnerNode* inner_node) const;
    virtual void visit(TermNode* term_node) const;
    void dispatch_visit(NodeIdentIface* unknown) const
    {
        switch(unknown->type())
        {
            case NodeIdentIface::TYPE_INNER_NODE:
                visit(dynamic_cast<InnerNode*>(unknown));
                break;
            case NodeIdentIface::TYPE_TERM_NODE:
                visit(dynamic_cast<TermNode*>(unknown));
                break;
        }
    }
};

struct ReverseVisitorDFS : public VisitorDFS
{
    void visit(InnerNode* inner_node) const;
    void visit(TermNode* term_node) const;
};

#endif
