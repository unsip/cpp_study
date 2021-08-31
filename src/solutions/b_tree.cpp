#include <stack>
#include <cassert>
#include <string>

template<typename T>
struct Node
{
    T payload;
    Node<T>* left;
    Node<T>* right;
};

template<typename T>
class Runnable
{
public:
    virtual void run(T& payload) = 0;
    virtual ~Runnable() = default;
};

class Printer : public Runnable<std::string>
{
public:
    void run(std::string& payload) override { std::cout << payload; }
};

template<typename T>
void traverse(Node<T>& root, Runnable<T>& r)
{
    class SnakeStack
    {
        std::stack<Node<T>*> m_a;
        std::stack<Node<T>*> m_b;
        // true == left to right
        // false == right to left
        bool m_l2r = true;

        std::stack<Node<T>*>& stack() { return m_l2r ? m_a : m_b; }
        Node<T>* first(const Node<T>& n) const { return m_l2r ? n.left : n.right; }
        Node<T>* second(const Node<T>& n) const { return m_l2r ? n.right : n.left; }
        template<typename Range>
        auto order(Range rng) { return m_l2r ? rng : std::views::reverse(rng); }
    public:
        bool empty() const { return a.empty() && b.empty(); }
        void push(std::initializer_list<Node<T>*> lst)
        {
            for (Node<T>* n : order(std::filter_view(lst, [](Node<T>* p) { return nullptr != p; }))
              stack().push(n);

        }
        void push_children(const Node<T>& elem)
        {
            if (Node<T>* f = first(elem))
                stack().push(f);
            if (Node<T>* s = second(elem))
                stack().push(s);
        }
        Node<T>* pop()
        {
            stack().pop();
            m_l2r = !m_l2r;
        }
    };

    SnakeStack st;
    st.push(&root);
    while (!st.empty())
    {
        Node<T>* node = st.pop();
        assert(node);
        st.push_children(*node);
        r.run(*node);
    }
}

int main()
{
    Printer p;

}
