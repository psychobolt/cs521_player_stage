#pragma once

template <class T>
class IModule
{
  public:
    static T& GetInstance()
    {
      static T instance;
      return instance;
    }
    void operator=(T const&);
    virtual void Run() = 0;
  protected:
    IModule() {}
    IModule(IModule const&);
    virtual ~IModule() {}
};
