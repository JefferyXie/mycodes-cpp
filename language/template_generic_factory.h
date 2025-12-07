#pragma once

#include "../main/header.h"

struct generic_factory_t {
    static generic_factory_t& instance()
    {
        static generic_factory_t inst;
        return inst;
    }

    typedef void* (*create_func_t)();
    std::map<std::string, create_func_t> registry_;

    void add_type(const std::string& name, create_func_t func)
    {
        std::cout << "add_type:" << name << std::endl;
        registry_[name] = func;
    }

    void* create(const std::string& name)
    {
        auto it = registry_.find(name);
        if (it != registry_.end()) {
            return it->second();
        }
        std::cout << "generic_factory_t::create: cannot find " << name << std::endl;
        return nullptr;
    }
};

//
// static variable in class template behaves differently from non-template,
// compiler won't instantiate (by calling template class's constructor) unless
// it's necessary (static variable instance is explicitly used in the code)
//
// https://stackoverflow.com/questions/18570632/implicit-initialization-of-static-member-variables-for-template-classes
// https://stackoverflow.com/questions/17132114/non-constant-static-members-in-template-generic-class-c-vs-c-sharp
//

template <class CLASS, class FACTORY>
struct generic_register_t {
    struct registor_t {
        registor_t()
        {
            std::cout << "reigstor()" << std::endl;
            FACTORY::instance().add_type(CLASS::Name(), create_func);
        }
    };

    static registor_t registor_;

//
// solution 1): provide a definition inside either constructor or destructor
// to enforce instantiation at the beginning
//
#define FORCE_INSTANTIATE(v) (v)
    // generic_register_t() {
    //     FORCE_INSTANTIATE(registor_);
    //     std::cout << "generic_register_t" << std::endl;
    // }
    ~generic_register_t()
    {
        FORCE_INSTANTIATE(registor_);
        std::cout << "~generic_register_t" << std::endl;
    }

    //
    // solution 2): use static variable in template to enforce instantiation
    //
    // template<registor_t*> class force_instantiation;
    // static force_instantiation<&registor_> notInstantiated_;

    static void* create_func() { return new CLASS(); }
};

template <class CLASS, class FACTORY>
typename generic_register_t<CLASS, FACTORY>::registor_t generic_register_t<CLASS, FACTORY>::registor_;
//

//
// in contrast, non-template class always instantiates static variables
//
struct non_template_A {
    struct B {
        B() { std::cout << "B()" << std::endl; }
    };
    static B b;
};
non_template_A::B non_template_A::b;
//

struct register_A : public generic_register_t<register_A, generic_factory_t> {
    static std::string Name() { return "register_A"; }
    register_A() { std::cout << "register_A() is called." << std::endl; }
};

struct register_B : public generic_register_t<register_B, generic_factory_t> {
    static std::string Name() { return "register_B"; }
    register_B() { std::cout << "register_B() is called." << std::endl; }
};

void run_template_generic_factory()
{
    generic_factory_t::instance().create("register_A");
    generic_factory_t::instance().create("register_B");
    generic_factory_t::instance().create("register_C");
}

