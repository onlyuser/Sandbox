#ifndef MAIN_H_
#define MAIN_H_

#include <string>

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
    virtual void visit_any(NodeIdentIface* unknown) = 0;
};

template<class T>
class Visitable
{
public:
    Visitable(T* instance) : m_instance(*instance)
    {}
    virtual ~Visitable()
    {}
    virtual void accept(VisitorIFace* v)
    {
        v->visit_any(&m_instance);
    }

private:
    T &m_instance;
};

struct TermNode : public NodeIdentIface, public Visitable<TermNode>
{
    TermNode(std::string name = "")
        : NodeIdentIface(NodeIdentIface::TYPE_TERM_NODE, name), Visitable<TermNode>(this)
    {}
};

struct InnerNode : public NodeIdentIface, public Visitable<InnerNode>
{
public:
    InnerNode(size_t n, std::string name = "")
        : NodeIdentIface(NodeIdentIface::TYPE_INNER_NODE, name), Visitable<InnerNode>(this), m_size(n)
    {
        m_children = new TermNode*[n];
        for(int i=0; i<static_cast<int>(n); i++)
            m_children[i] = new TermNode();
    }
    ~InnerNode()
    {
        if(m_children)
        {
            for(int i=0; i<static_cast<int>(m_size); i++)
            {
                if(m_children[i])
                    delete m_children[i];
            }
            delete[] m_children;
        }
    }
    TermNode* child(int index)
    {
        return m_children[index];
    }
    size_t size() const
    {
        return m_size;
    }

private:
    TermNode** m_children;
    size_t m_size;
};

struct VisitorDFS : public VisitorIFace
{
    VisitorDFS()
    {}
    virtual void visit(InnerNode* inner_node);
    virtual void visit(TermNode* term_node);
    void visit_any(NodeIdentIface* unknown)
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
    void visit(InnerNode* inner_node);
    void visit(TermNode* term_node);
};

#endif
