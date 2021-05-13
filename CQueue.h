//
// Created by mine268 on 2021/5/13.
//

#ifndef CPP_LIB_CQUEUE_H
#define CPP_LIB_CQUEUE_H

#include <cassert>

/**
 * 自定义的队列，允许遍历，其中所有的元素都是一个副本，修改队列中的元素不会对原有元素产生影响。所有储存再这个队列中元素都*必须*有一个缺省的构造函数。
 * @tparam type 表示存储元素的类型
 */
template<typename type>
class CQueue {
private:
    /**
     * 队列中的单位元素类，包含前向指针和后向指针。允许通过两种初始化初始元素内容，初始化的方式包括拷贝复制和移动复制。
     */
    struct Element;

    /**
     * 队列的迭代器。
     */
    class iterator;

protected:
    /**
     * head 队列对头的元素
     * tail 队列队尾的元素
     * count 队列中的所有元素个数
     * 队列本来就拥有一个哨兵元素作为队首，所有元素跟在这个元素后头，当head于tail指向相同元素的时候，队列为空
     */
    Element *head, *tail;
    std::size_t count;

public:
    /**
     * 构造函数，初始化队列，即插入一个哨兵元素。
     */
    CQueue();

    /**
     * 删除拷贝构造
     */
    CQueue(const CQueue<type>&) = delete;

    /**
     * 删除移动构造
     */
    CQueue(CQueue<type>&&) = delete;

    /**
     * 析构函数，释放队列的所有资源。
     */
    ~CQueue();

    virtual void push(const type &ele);

    virtual void push(type &&ele);

    virtual void pop();

    virtual type top();

    virtual bool empty();

    /**
     * 删除队列中的元素，该元素的位置由迭代器x指定
     * @param x 要删除的元素的迭代器
     */
    virtual void remove(CQueue<type>::iterator &x);

    /**
     * 返回一个指向迭代器，指向当前的队列第一个元素，如果没有元素则指向空。
     * @return 如上
     */
    iterator begin() const;

    /**
     * 返回指向空元素的迭代器
     * @return
     */
    iterator end() const;
};

template<typename type>
CQueue<type>::CQueue() : count(0) {
    tail = head = new Element;
}

template<typename type>
CQueue<type>::~CQueue() {
    Element *cur = head;
    while (head != tail) {
        head = cur->next;
        delete head;
        cur = head;
    }
}

template<typename type>
void CQueue<type>::push(const type &ele) {
    auto *tmp = new Element(ele);
    tail->next = tmp;
    tmp->prev = tail;
    tail = tail->next;
    ++count;
}

template<typename type>
void CQueue<type>::push(type &&ele) {
    auto *tmp = new Element(ele);
    tail->next = tmp;
    tmp->prev = tail;
    tail = tail->next;
    ++count;
}

template<typename type>
void CQueue<type>::pop() {
    assert(count > 0);
    if (count > 0) {
        head = head->next;
        delete head->prev;
        head->prev = nullptr;
        --count;
    }
}

template<typename type>
type CQueue<type>::top() {
    assert(count > 0);
    if (count > 0)
        return head->next->res;
}

template<typename type>
bool CQueue<type>::empty() {
    return count == 0;
}

template<typename type>
void CQueue<type>::remove(CQueue<type>::iterator &x) {
    if (count == 0 || x.cur == nullptr) return;
    else {
        if (x.cur == head->next) {
            auto tmp = head->next;
            head->next = head->next->next;
            if (head->next != nullptr)
                head->next->prev = head;
            else {
                tail = head;
                head->next = nullptr;
            }
            delete tmp;
            --count;
        } else if (x.cur == tail) {
            x.cur = x.cur->prev;
            tail = x.cur;
            delete tail->next;
            x.cur->next = nullptr;
            --count;
        } else {
            auto tmp = x.cur;
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
            delete tmp;
            --count;
        }
    }
}

template<typename type>
typename CQueue<type>::iterator CQueue<type>::begin() const {
    if (head == nullptr)
        return iterator(nullptr);
    else
        return iterator(head->next);
}

template<typename type>
typename CQueue<type>::iterator CQueue<type>::end() const {
    return iterator(nullptr);
}


template<typename type>
struct CQueue<type>::Element {
    friend class CQueue<type>;

    type res;
    Element *prev, *next;

    /**
     * 默认构造
     */
    Element() : res(), prev(nullptr), next(nullptr) {}

    /**
     * 拷贝构造函数
     * @param x 如题
     */
    explicit Element(const type &x) : res(x), prev(nullptr), next(nullptr) {}

    /**
     * 移动构造函数
     * @param x 如题
     */
    explicit Element(type &&x) : res(x), prev(nullptr), next(nullptr) {}
};

template<typename type>
class CQueue<type>::iterator {
    friend class CQueue<type>;

    Element *cur;
public:
    /**
     * 构造函数
     * @param c 当前迭代器指定的队列元素的指针
     */
    explicit iterator(Element *c) : cur(c) {}

    iterator(const iterator &x) = default;

    friend bool operator==(const iterator &x, const iterator &y) {
        return x.cur == y.cur;
    }

    friend bool operator!=(const iterator &x, const iterator &y) {
        return x.cur != y.cur;
    }

    iterator &operator=(const iterator &) = default;

    iterator operator++() {
        moveBack();
        return *this;
    }

    iterator operator++(int) {
        iterator copy = *this;
        moveBack();
        return copy;
    }

    iterator operator--() {
        moveFront();
        return *this;
    }

    iterator operator--(int) {
        iterator copy = *this;
        moveFront();
        return copy;
    }

    /**
     * 返回当前迭代器指定的元素
     * @return 指定的元素
     */
    type fetch() {
        return cur->res;
    }

    /**
     * 迭代器后移
     */
    void moveBack() {
        if (cur != nullptr)
            cur = cur->next;
    }

    /**
     * 迭代器前移
     */
    void moveFront() {
        if (cur != nullptr)
            cur = cur->prev;
    }

    /**
     * 当前迭代器是否为空了
     * @return 是否为空，是为`true`，否为`false`
     */
    bool isNull() {
        return cur == nullptr;
    }
};

#endif //CPP_LIB_CQUEUE_H
