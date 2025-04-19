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
#include <filesystem>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

void help()
{
    std::cout << "yajuiku  +  bootstrap     で環境構築(Linux)(開発中)\n"
    << "yajuiku  +  build     でビルド\n"
    << "yajuiku  +  run     で実行(開発中)\n"
    << "yajuiku  +  yajuiku     でyajuikuをビルド\n"
    << "yajuiku  +  builrun     でビルドと実行(開発中)\n"
    << "yajuiku  +  install     でこのコンピューターにインストール(実装予定)\n"
    << "yajuiku  +  ruun     でこのコンピューターにインストールされたMasoRPGを実行\n"
    << "yajuiku  +  remove     でこのコンピューターにインストールされたMasoRPGを削除(実装予定)\n"
    << "yajuiku  + reset     で、ビルドの設定とか削除" << std::endl;
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

void build(std::filesystem::path compilerPath)
{
    std::filesystem::path dataPath = compilerPath / "run" / "data";
    std::filesystem::path binPath = compilerPath / "run" / "bin" / "main";

    std::string command = "g++ -std=c++17 -o " + binPath.string() +
        " src/main.cpp src/Camera2D.cpp $(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)";
    system(command.c_str());

    try {
        std::filesystem::copy("fonts", dataPath / "fonts",
            std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
        std::filesystem::copy("image", dataPath / "image",
            std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
        std::filesystem::copy("music", dataPath / "music",
            std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
        std::filesystem::copy("fentanyL", compilerPath / "run" / "bin" / "fentanyL",
            std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
            
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "コピーエラー: " << e.what() << std::endl;
    }
}

void run()
{
    std::cout << "※開発中" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    fs::path execPath = fs::current_path() / "compiler" / "run" / "bin" / "main";
    std::string command = execPath.string() + " debug";
    system(command.c_str());
}

void yajuiku()
{
    fs::path outputPath = fs::current_path() / "yajuiku";
    fs::path sourcePath = fs::current_path() / "compiler" / "src" / "main.cpp";

    std::string command = "g++ -std=c++17 " + sourcePath.string() + " -o " + outputPath.string();
    system(command.c_str());
}

void install(std::filesystem::path compilerPath)
{
    std::string command = "sudo cp -r " + compilerPath.string() + " /opt/masoRpgDebugData";
    system(command.c_str());
}

void remove()
{
    std::string command = "sudo rm -rf /opt/masoRpgDebugData";
    system(command.c_str());
}

void ruun()
{
    system("./opt/masorpg/bin/main");
}

void reset()
{
    fs::path compilerPath = fs::current_path() / "compiler";
    fs::path runPath = compilerPath / "run";
    fs::path samplePath = compilerPath / "sample";

    try {
        // compiler/run を削除
        if (fs::exists(runPath)) {
            fs::remove_all(runPath);
        }

        // compiler/sample を compiler/run にコピー
        fs::copy(samplePath, runPath,
            fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "リセットエラー: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::filesystem::path basePath = std::filesystem::current_path();
    std::filesystem::path compilerPath = basePath / "compiler";
    std::vector<std::string> args(argv + 1, argv + argc);
    for (const std::string& arg : args) {
        if (arg == "bootstrap") bootstrap();
        else if (arg == "build")
        {
            reset();
            build(compilerPath);
        }
        else if (arg == "run") run();
        else if (arg == "help") help();
        else if (arg == "yajuiku") yajuiku();
        else if (arg == "builrun")
        {
            reset();
            build(compilerPath);
            run();
        }
        else if (arg == "install")
        {
            reset();
            install(compilerPath);
            // build();
        }
        else if (arg == "remove") remove();
        else if ((arg == "reset")) reset();
        else if ((arg == "ruun")) ruun();
    }
    return 0;
}
