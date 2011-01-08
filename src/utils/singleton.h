#ifndef UTILS_SINGLETON_H
#define UTILS_SINGLETON_H

/// A singleton class. I'm sorry to have this, but it makes it easier for the
/// service providing modules. Use with extreme caution.
template<typename DerivedT>
class Singleton {
public:
  static DerivedT &instance() {
    static DerivedT inst;
    return inst;
  }
};

#endif // !UTILS_SINGLETON_H
