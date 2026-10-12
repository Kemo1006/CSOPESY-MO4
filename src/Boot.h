#ifndef BOOT_H
#define BOOT_H

// Boot sequence shown before the desktop: a BIOS/POST screen followed by the
// CSOPESY loading screen. Both are full-screen black ImGui windows.
class BootSequence {
public:
    BootSequence();

    // Renders the current boot screen. Returns true once the sequence has finished.
    bool UpdateAndRender();

private:
    enum Phase { PHASE_BIOS, PHASE_LOADING, PHASE_DONE };

    Phase m_phase;
    float m_phaseTime;

    void RenderBios(float duration);
    void RenderLoading(float duration);
    bool SkipRequested() const;
};

#endif // BOOT_H
