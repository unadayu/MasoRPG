/*
  Hamster-crabが急いで作ったg++ベースのコンパイラーです。
  たぶんインストールのところのパッケージをAIで調べたので多分違います。
  なにか問題があったら
  hamstercrab123@gmail.com
  までメールくれ
**/

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

void help()
{
    std::cout << "yajuiku  +  bootstrap     で環境構築(Linux)(開発中)\n"
    << "yajuiku  +  build     でビルド\n"
    << "yajuiku  +  run     で実行(開発中)\n"
    << "yajuiku  +  yajuiku     でyajuikuをビルド\n"
    << "yajuiku  +  builrun     でビルドと実行(開発中)"
    << "yajuiku  +  install     でこのコンピューターにインストール(実装予定)"
    << "yajuiku  +  remove     でこのコンピューターにインストールされたMasoRPGを削除(実装予定)" << std::endl;
}

void bootstrap()
{
        auto command_exists = [](const std::string& cmd) -> bool {
        return std::system((cmd + " --version > /dev/null 2>&1").c_str()) == 0;
    };

    if (command_exists("g++")) {
        std::cout << "g++ is already installed.\n";
        return;
    }

    std::cout << "g++ not found. Attempting to install...\n";

    if (command_exists("apt")) {
        std::system("sudo apt update && sudo apt install -y g++");
    } else if (command_exists("dnf")) {
        std::system("sudo dnf install -y g++");
    } else if (command_exists("yum")) {
        std::system("sudo yum install -y gcc-c++");
    } else if (command_exists("pacman")) {
        std::system("sudo pacman -Sy --noconfirm gcc");
    } else if (command_exists("zypper")) {
        std::system("sudo zypper install -y gcc-c++");
    } else {
        std::cerr << "Unsupported package manager. Please install g++ manually.\n";
    }
}

void build()
{
    system("rm -rf compiler/run");
    system("cp -r compiler/sample compiler/run");
    system("g++ -std=c++17 -o ./compiler/run/bin/main src/main.cpp src/Camera2D.cpp $(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)");
    system("cp -r fonts compiler/run/data");
    system("cp -r image compiler/run/data");
    system("cp -r music compiler/run/data");
}

void run()
{
    std::cout << "※開発中" << std::endl;
    system("sleep 1");
    system("./compiler/run/bin/main");
}

void yajuiku()
{
    system("g++ -std=c++17 compiler/src/main.cpp -o ./yajuiku");
}

void install()
{
    std::cout << "まだ" << std::endl;
}

void remove()
{
    std::cout << "まだ" << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    for (const std::string& arg : args) {
        if (arg == "bootstrap") bootstrap();
        else if (arg == "build") build();
        else if (arg == "run") run();
        else if (arg == "help") help();
        else if (arg == "yajuiku") yajuiku();
        else if (arg == "builrun")
        {
            build();
            run();
        }
        else if (arg == "install") install();
        else if (arg == "remove") remove();
    }
    return 0;
}
