#include <iostream>
#include <string>
using namespace std;

//DevelopmentTool
class DevelopmentTool {
public:
    virtual string getName() = 0;
    virtual ~DevelopmentTool() {}
};

class Delphi : public DevelopmentTool {
public:
    string getName() {
        return "Delphi";
    }
};

class VisualStudio : public DevelopmentTool {
public:
    string getName() {
        return "Visual Studio";
    }
};

class NetBeans : public DevelopmentTool {
public:
    string getName() {
        return "NetBeans";
    }
};


//OperatingSystem
class OperatingSystem {
public:
    virtual string getName() = 0;
    virtual ~OperatingSystem() {}
};

class WinXP : public OperatingSystem {
public:
    string getName() {
        return "Windows XP";
    }
};

class Win8 : public OperatingSystem {
public:
    string getName() {
        return "Windows 8";
    }
};

class Linux : public OperatingSystem {
public:
    string getName() {
        return "Linux";
    }
};

//Compiler
class Compiler {
public:
    virtual string getName() = 0;
    virtual ~Compiler() {}
};

class Borland : public Compiler {
public:
    string getName() {
        return "Borland";
    }
};

class Visual : public Compiler {
public:
    string getName() {
        return "Visual";
    }
};

class MinGW : public Compiler {
public:
    string getName() {
        return "MinGW";
    }
};

// Абстрактный класс для создания средств разработки, операционных систем и компиляторов
class AbstractFactory {
public:
    virtual DevelopmentTool* createDevelopmentTool() = 0;
    virtual OperatingSystem* createOperatingSystem() = 0;
    virtual Compiler* createCompiler() = 0;
    virtual string getName() = 0;
    virtual ~AbstractFactory() {}
};

// Конкретная фабрика для старых лицензий
class OldLicenseFactory : public AbstractFactory {
public:
    DevelopmentTool* createDevelopmentTool() {
        return new Delphi();
    }
    OperatingSystem* createOperatingSystem() {
        return new WinXP();
    }
    Compiler* createCompiler() {
        return new Borland();
    }
    string getName() {
        return "OldLicenseFactory";
    }
};

// Конкретная фабрика для новых лицензий
class NewLicenseFactory : public AbstractFactory {
public:
    DevelopmentTool* createDevelopmentTool() {
        return new VisualStudio();
    }
    OperatingSystem* createOperatingSystem() {
        return new Win8();
    }
    Compiler* createCompiler() {
        return new Visual();
    }
    string getName() {
        return "OldLicenseFactory";
    }
};

// Конкретная фабрика для открытых лицензий
class OpenLicenseFactory : public AbstractFactory {
public:
    DevelopmentTool* createDevelopmentTool() {
        return new NetBeans();
    }
    OperatingSystem* createOperatingSystem() {
        return new Linux();
    }
    Compiler* createCompiler() {
        return new MinGW();
    }
    string getName() {
        return "OldLicenseFactory";
    }
};


int main() {
    AbstractFactory* factory;

    // Создаем фабрику для старых лицензий
    factory = new OldLicenseFactory();
    cout << "Factory: " << factory->getName() << endl;
    cout << "Development Tool: " << factory->createDevelopmentTool()->getName() << endl;
    cout << "Operating System: " << factory->createOperatingSystem()->getName() << endl;
    cout << "Compiler: " << factory->createCompiler()->getName() << endl;
    delete factory;

    cout << "-------------------------" << endl;

    // Создаем фабрику для новых лицензий
    factory = new NewLicenseFactory();
    cout << "Factory: " << factory->getName() << endl;
    cout << "Development Tool: " << factory->createDevelopmentTool()->getName() << endl;
    cout << "Operating System: " << factory->createOperatingSystem()->getName() << endl;
    cout << "Compiler: " << factory->createCompiler()->getName() << endl;
    delete factory;

    cout << "-------------------------" << endl;

    // Создаем фабрику для открытых лицензий
    factory = new OpenLicenseFactory();
    cout << "Factory: " << factory->getName() << endl;
    cout << "Development Tool: " << factory->createDevelopmentTool()->getName() << endl;
    cout << "Operating System: " << factory->createOperatingSystem()->getName() << endl;
    cout << "Compiler: " << factory->createCompiler()->getName() << endl;
    delete factory;
    return 0;

}