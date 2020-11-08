class Dummy
{
private:
    struct Impl
    {
        Impl(const Smth& a, const Foo& b, const Bar& c, const Boi& d)
            :   m_smth(a),
                m_foo(b),
                m_bar(c),
                m_boi(d)
        {}

        Smth m_smth;
        Foo m_foo;
        Bar m_bar;
        Boi m_boi;
    };

    Impl* m_impl;
public:
    Dummy(const Smth& a, const Foo& b, const Bar& c, const Boi& d)
        : m_impl(new Impl(a, b, c, d))
    {}
    Dummy(const Dummy& rhv)
    {
        *m_impl = *rhv.m_impl;
    }

    ~Dummy()
    {
        delete m_impl;
    }

    Dummy& operator =(const Dummy& rhv)
    {
        Dummy tmp {rhv};
        this->swap(tmp);
        return *this;
    };

    void swap(Dummy& rhv) noexcept
    {
        std::swap(rhv.m_impl, m_impl);
    };
};
