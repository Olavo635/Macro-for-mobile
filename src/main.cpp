#include <pl/Mod.hpp>
#include <imgui.h>
#include <vector>
#include <string>

// Definição de keycodes comuns do Android NDK
namespace AndroidKeys {
    constexpr int KEY_ALT_LEFT = 57; // AKEYCODE_ALT_LEFT
    constexpr int KEY_0        = 7;
    constexpr int KEY_1        = 8;
    constexpr int KEY_2        = 9;
    constexpr int KEY_3        = 10;
    constexpr int KEY_4        = 11;
    constexpr int KEY_X        = 52; // AKEYCODE_X
    constexpr int KEY_C        = 31; // AKEYCODE_C
    constexpr int KEY_V        = 50; // AKEYCODE_V
    constexpr int KEY_Z        = 54; // AKEYCODE_Z
}

struct MacroButtonConfig {
    bool enabled = true;
    std::string label = "Macro 1";
    int targetKeyCode = AndroidKeys::KEY_1;
};

class MacroButtonsMod {
public:
    static MacroButtonsMod& instance() {
        static MacroButtonsMod inst;
        return inst;
    }

    MacroButtonsMod() : mSelf(*ll::mod::NativeMod::current()) {}

    bool load() {
        // Inicializa até 4 macros
        mMacros.resize(4);
        mMacros[0] = {true, "Alt+1", AndroidKeys::KEY_1};
        mMacros[1] = {true, "Alt+2", AndroidKeys::KEY_2};
        mMacros[2] = {true, "Alt+3", AndroidKeys::KEY_3};
        mMacros[3] = {true, "Alt+4", AndroidKeys::KEY_4};
        return true;
    }

    bool enable() {
        mSelf.getLogger().info("MacroButtonsMod ativado!");
        return true;
    }

    bool disable() {
        return true;
    }

    // 1. Simula envio de Tecla Única (sem Alt) para configuração no jogo
    void sendSingleKey(int keyCode) {
        ll::input::sendKeyEvent(keyCode, ll::input::KeyState::Down);
        ll::input::sendKeyEvent(keyCode, ll::input::KeyState::Up);
    }

    // 2. Simula o atalho completo: Alt + Tecla
    void triggerAltMacro(int keyCode) {
        // Pressiona Alt
        ll::input::sendKeyEvent(AndroidKeys::KEY_ALT_LEFT, ll::input::KeyState::Down);

        // Pressiona e solta a tecla alvo
        ll::input::sendKeyEvent(keyCode, ll::input::KeyState::Down);
        ll::input::sendKeyEvent(keyCode, ll::input::KeyState::Up);

        // Solta o Alt
        ll::input::sendKeyEvent(AndroidKeys::KEY_ALT_LEFT, ll::input::KeyState::Up);
    }

    // Renderiza a interface de configuração no Mod Menu
    void renderConfigUI() {
        ImGui::Text("Configuracao dos 4 Botoes de Macro");
        ImGui::Separator();

        for (int i = 0; i < 4; ++i) {
            ImGui::PushID(i);
            ImGui::Checkbox("Ativar", &mMacros[i].enabled);
            ImGui::SameLine();

            char labelBuf[32];
            strncpy(labelBuf, mMacros[i].label.c_str(), sizeof(labelBuf));
            if (ImGui::InputText("Rotulo", labelBuf, sizeof(labelBuf))) {
                mMacros[i].label = labelBuf;
            }

            ImGui::InputInt("KeyCode", &mMacros[i].targetKeyCode);
            ImGui::SameLine();

            // BOTAO ESPECIAL: Envia apenas a tecla avulsa para vincular no jogo!
            if (ImGui::Button("Simular Tecla (para Config)")) {
                sendSingleKey(mMacros[i].targetKeyCode);
            }
            ImGui::Separator();
            ImGui::PopID();
        }
    }

    // Renderiza o HUD flutuante in-game
    void renderHUD() {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                                ImGuiWindowFlags_AlwaysAutoResize | 
                                ImGuiWindowFlags_NoBackground;

        ImGui::SetNextWindowPos(ImVec2(20, 200), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("##MacroButtonsOverlay", nullptr, flags)) {
            for (int i = 0; i < 4; ++i) {
                if (!mMacros[i].enabled) continue;

                ImGui::PushID(i);
                // Botão touch-friendly de 70x70 pixels
                if (ImGui::Button(mMacros[i].label.c_str(), ImVec2(70, 70))) {
                    triggerAltMacro(mMacros[i].targetKeyCode);
                }
                ImGui::PopID();
                ImGui::SameLine();
            }
        }
        ImGui::End();
    }

private:
    ll::mod::NativeMod& mSelf;
    std::vector<MacroButtonConfig> mMacros;
};

// Registro do mod no LeviLauncher
PL_REGISTER_MOD(MacroButtonsMod, MacroButtonsMod::instance());
