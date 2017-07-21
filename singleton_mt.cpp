//This file is part of Foobar.
//
//    Foobar is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Foobar is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

// It behaves as non-thread safe variant.
// Only difference is that threre is 
//   only one call to constructor in multithreaded aplications.
// 
// After constor call there is no difference between standart implementation!

#include <iostream>
#include <mutex>

class Singleton {
    public:
        // get instance
        static Singleton* Instance();
    protected:
        static Singleton* get_first ();
        static Singleton* get_after ();

        // constructor
        Singleton();
    private:
        // location of maximum one instance (nullptr otherwise)
        static Singleton* _instance;

        static Singleton* (*act_behavior) ();
        // mutex for get_first
        static std::mutex mtx;
};

Singleton*  Singleton::_instance = nullptr;
Singleton*  (*Singleton::act_behavior) () = &Singleton::get_first;
std::mutex  Singleton::mtx; 

// thread safe construction
Singleton* Singleton::get_first () {
    // get instance for first few acesses
    std::lock_guard<std::mutex> lck (Singleton::mtx);

    if ( _instance == nullptr ) {
        _instance = new Singleton;
        act_behavior = &get_after;
    }
    return _instance;
}

// get instance without condition
Singleton* Singleton::get_after () {
    // get instance after first initialization
    return _instance;
}

// get instance
Singleton* Singleton::Instance() {
    return (*act_behavior)();
}

// constructor
Singleton::Singleton () {
    std::cout << "Singleton::Singleton()" << std::endl;
}
