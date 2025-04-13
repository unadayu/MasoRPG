#include <iostream>
#include <cstdlib>
#include <unistd.h> // for access() and chdir()
#include <filesystem>

namespace fs = std::filesystem;

bool command_exists(const char* cmd) {
    std::string check = std::string("command -v ") + cmd + " > /dev/null 2>&1";
    return system(check.c_str()) == 0;
}

bool install_with_package_manager(const char* cmd_name) {
    if (command_exists("apt")) {
        std::cout << cmd_name << " が見つかりません。apt を使ってインストールを試みます...\n";
        return system((std::string("sudo apt update && sudo apt install -y ") + cmd_name).c_str()) == 0;
    } else if (command_exists("dnf")) {
        std::cout << cmd_name << " が見つかりません。dnf を使ってインストールを試みます...\n";
        return system((std::string("sudo dnf install -y ") + cmd_name).c_str()) == 0;
    } else if (command_exists("pacman")) {
        std::cout << cmd_name << " が見つかりません。pacman を使ってインストールを試みます...\n";
        return system((std::string("sudo pacman -Sy --noconfirm ") + cmd_name).c_str()) == 0;
    } else {
        std::cerr << "対応するパッケージマネージャーが見つかりません。" << cmd_name << " を手動でインストールしてください。\n";
        return false;
    }
}

int main() {
    if (!command_exists("cmake")) {
        if (!install_with_package_manager("cmake")) {
            std::cerr << "cmake のインストールに失敗しました。処理を中断します。\n";
            return 1;
        }
    }

    std::cout << "build ディレクトリを作成しています...\n";
    if (fs::exists("build")) {
        std::cout << "build ディレクトリを削除します...\n";
        fs::remove_all("build");
    }
    system("mkdir build");

    if (chdir("build") != 0) {
        std::cerr << "build ディレクトリへの移動に失敗しました。\n";
        return 1;
    }

    std::cout << "cmake を実行しています...\n";
    int result = system("cmake ..");
    if (result != 0) {
        std::cerr << "cmake の実行に失敗しました（終了コード: " << result << "）。\n";
        return 1;
    }

    std::cout << "cmake の実行が正常に完了しました。\n";

    system("make");
    system("mv main ../main");
    system("cd ..");
    if (fs::exists("build")) {
        std::cout << "build ディレクトリを削除します...\n";
        fs::remove_all("build");
    }
    system("sleep 1");
    std::cout << "ビルドに成功しました\n 実行するには | ./main | とだけ打ってください。" << std::endl;
    return 0;
}
