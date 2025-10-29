#pragma once

#include "../main/header.h"

class control_block_t
{
public:
    void increase_ref() { ++ref_count_; }
    void decrease_ref()
    {
        assert(ref_count_ > 0);
        if (!--ref_count_) {
            on_ref_count_zero();
            delete this;
        }
    }
    int use_count() { return ref_count_; }

    virtual void on_ref_count_zero() = 0;
    virtual ~control_block_t(){};

private:
    int ref_count_{0};

    // TODO: generic allocator, deallocator...
    // Alloc alloc_;
    // Dealloc dealloc_;
};

template <class T>
class my_smartpointer
{
public:
    template <typename U>
    struct control_block_pointer_t : public control_block_t {
        control_block_pointer_t(U* obj) : object_(obj) {}

        void on_ref_count_zero() override
        {
            delete object_;
            object_ = nullptr;
        }

        U* object_{nullptr};
    };

    template <typename U>
    struct control_block_object_t : public control_block_t {
        template <typename... Args>
        control_block_object_t(Args&&... args) : object_{std::forward<Args>(args)...}
        {
        }

        void on_ref_count_zero() override {}

        U object_;
    };

public:
    my_smartpointer() = default;

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    my_smartpointer(U* obj)
    {
        if (obj) {
            object_     = obj;
            ctrl_block_ = new control_block_pointer_t<U>{obj};
            ctrl_block_->increase_ref();
        }
    }

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    my_smartpointer(control_block_object_t<U>* ctrl)
    {
        if (ctrl) {
            object_     = &ctrl->object_;
            ctrl_block_ = ctrl;
            ctrl_block_->increase_ref();
        }
    }

    // NOTICE: this explicit copy and move constructors are necessary, because -
    // 1) by c++ grammar, template function will never be copy or move constructors! The following templates are
    //    actually template constructor rather than template copy constructor.
    // 2) compiler always generate implicit copy and move constructors if possible, and by c++ discripline, non-template
    //    functions (i.e. overload) takes previlige over template functions if both match.
    //
    // https://stackoverflow.com/questions/55845896/why-doesnt-the-standard-consider-a-template-constructor-as-a-copy-constructor
    // https://stackoverflow.com/questions/32537994/c-template-copy-constructor-on-template-class
    my_smartpointer(const my_smartpointer& o)
    {
        if (o.ctrl_block_) {
            object_     = o.object_;
            ctrl_block_ = o.ctrl_block_;
            ctrl_block_->increase_ref();
        }
    }

    template <typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>, void>>
    my_smartpointer(const my_smartpointer<U>& o)
    {
        if (o.ctrl_block_) {
            object_     = o.object_;
            ctrl_block_ = o.ctrl_block_;
            ctrl_block_->increase_ref();
        }
    }

    // NOTICE: same reasoning as above
    my_smartpointer& operator=(const my_smartpointer& o)
    {
        my_smartpointer{o}.swap(*this);
        return *this;
    }

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    my_smartpointer& operator=(const my_smartpointer<U>& o)
    {
        my_smartpointer{o}.swap(*this);
        return *this;
    }

    // NOTICE: same reasoning as above
    my_smartpointer(my_smartpointer&& o)
    {
        object_     = o.object_;
        ctrl_block_ = o.ctrl_block_;

        o.object_     = nullptr;
        o.ctrl_block_ = nullptr;
    }

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    my_smartpointer(my_smartpointer<U>&& o)
    {
        object_     = o.object_;
        ctrl_block_ = o.ctrl_block_;

        o.object_     = nullptr;
        o.ctrl_block_ = nullptr;
    }

    // NOTICE: same reasoning as above
    my_smartpointer& operator=(my_smartpointer&& o)
    {
        my_smartpointer{std::move(o)}.swap(*this);
        return *this;
    }

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    my_smartpointer& operator=(my_smartpointer<U>&& o)
    {
        my_smartpointer{std::move(o)}.swap(*this);
        return *this;
    }

    ~my_smartpointer()
    {
        if (ctrl_block_) {
            ctrl_block_->decrease_ref();
        }
    }

    T* operator->() { return get(); }
    T& operator*() { return *get(); }
    T* get() { return object_; }

    template <typename U, std::enable_if_t<std::is_convertible_v<U, T>>* = nullptr>
    void reset(U* p)
    {
        my_smartpointer{p}.swap(*this);
    }
    void reset() { my_smartpointer{}.swap(*this); }
    void swap(my_smartpointer& o)
    {
        std::swap(object_, o.object_);
        std::swap(ctrl_block_, o.ctrl_block_);
    }
    int use_count() const { return ctrl_block_ ? ctrl_block_->use_count() : 0; }

private:
    T*               object_{nullptr};
    control_block_t* ctrl_block_{nullptr};

    // A trick that makes above member variables accessible by other my_smartpointer<U>
    template <typename U>
    friend class my_smartpointer;
};

template <typename T, typename... Args>
auto my_make_shared(Args&&... args)
{
    using control_t = typename my_smartpointer<T>::template control_block_object_t<T>;
    auto cntl_block = new control_t{std::forward<Args>(args)...};
    return my_smartpointer<T>(cntl_block);
}

