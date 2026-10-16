#include <QCoreApplication>
#include <QDir>
#include <QEvent>
#include <QFileInfo>
#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPalette>
#include <QProcess>
#include <QPushButton>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>
#include <QWindow>

#if defined(Q_OS_MACOS)
#include <objc/message.h>
#include <objc/runtime.h>
#endif

namespace {

class TitlebarDragFilter final : public QObject {
    QWidget &window;

public:
    explicit TitlebarDragFilter(QWidget &window) : window(window) {}

    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() != QEvent::MouseButtonPress) return false;
        auto *mouse = static_cast<QMouseEvent *>(event);
        if (mouse->button() != Qt::LeftButton) return false;
        auto *source = qobject_cast<QWidget *>(watched);
        if (!source || (source != &window && !window.isAncestorOf(source))) return false;

        const QPoint position = source->mapTo(&window, mouse->position().toPoint());
        if (position.y() >= 44) return false;
        if (auto *nativeWindow = window.windowHandle()) nativeWindow->startSystemMove();
        return true;
    }
};

struct Example {
    const char *name;
    const char *executable;
    const char *script;
};

constexpr Example examples[] = {
    {"Lettuce", SERENITY_LETTUCE_EXECUTABLE, nullptr},
    {"Input", SERENITY_INPUT_EXECUTABLE, nullptr},
    {"Camera", SERENITY_CAMERA_EXECUTABLE, nullptr},
    {"Don't Look Back (Lua)", SERENITY_RUNTIME_EXECUTABLE, SERENITY_DONTLOOKBACK_SCRIPT},
    {"Runtime", SERENITY_RUNTIME_EXECUTABLE, SERENITY_RUNTIME_TEST_SCRIPT},
};

QString workspaceRootFromExecutablePath() {
    const QString executableDir = QCoreApplication::applicationDirPath();
    for (const auto &outputDir : {QStringLiteral("/bazel-out/"), QStringLiteral("/bazel-bin/")}) {
        const auto marker = executableDir.lastIndexOf(outputDir);
        if (marker >= 0) return executableDir.left(marker);
    }
    return QDir::currentPath();
}

#if defined(Q_OS_MACOS)
void configureTransparentTitlebar(QWidget &widget) {
    id nativeView = reinterpret_cast<id>(widget.winId());
    using GetWindow = id (*)(id, SEL);
    id nativeWindow = reinterpret_cast<GetWindow>(objc_msgSend)(nativeView, sel_registerName("window"));
    if (!nativeWindow) return;

    using SetBool = void (*)(id, SEL, BOOL);
    reinterpret_cast<SetBool>(objc_msgSend)(nativeWindow,
        sel_registerName("setTitlebarAppearsTransparent:"), YES);
    reinterpret_cast<SetBool>(objc_msgSend)(nativeWindow,
        sel_registerName("setMovableByWindowBackground:"), YES);

    using GetStyleMask = unsigned long (*)(id, SEL);
    unsigned long styleMask = reinterpret_cast<GetStyleMask>(objc_msgSend)(nativeWindow,
        sel_registerName("styleMask"));
    using SetStyleMask = void (*)(id, SEL, unsigned long);
    reinterpret_cast<SetStyleMask>(objc_msgSend)(nativeWindow,
        sel_registerName("setStyleMask:"), styleMask | (1UL << 15));
}
#endif

void launch(const Example &example, const QString &workspaceRoot) {
    const QString executable = QDir(workspaceRoot).filePath(
        QString::fromUtf8(example.executable));
    if (!QFileInfo(executable).isExecutable()) {
        QMessageBox::warning(nullptr, QStringLiteral("Example not built"),
            QStringLiteral("Could not find the Bazel executable:\n%1\n\nBuild it with Bazel, then try again.").arg(executable));
        return;
    }

    QStringList arguments;
    if (example.script) arguments << QString::fromUtf8(example.script);
    if (!QProcess::startDetached(executable, arguments, workspaceRoot)) {
        QMessageBox::warning(nullptr, QStringLiteral("Could not launch example"),
            QStringLiteral("Failed to start %1.").arg(executable));
    }
}

}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    app.setApplicationName("Serenity Examples");
    app.setApplicationDisplayName("Serenity Examples");

    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#171a19"));
    palette.setColor(QPalette::WindowText, QColor("#f0f2e9"));
    palette.setColor(QPalette::Base, QColor("#202522"));
    palette.setColor(QPalette::Button, QColor("#252d29"));
    palette.setColor(QPalette::ButtonText, QColor("#f0f2e9"));
    palette.setColor(QPalette::Highlight, QColor("#a8d35f"));
    palette.setColor(QPalette::HighlightedText, QColor("#11140f"));
    app.setPalette(palette);
    app.setStyleSheet(R"(
        QWidget { font-family: "monospace"; font-size: 14px; }
        QFrame#card { background: #202522; border: 1px solid #354139; border-radius: 10px; }
        QLabel#eyebrow { color: #a8d35f; font-size: 11px; font-weight: 700; letter-spacing: 1px; }
        QLabel#title { color: #f0f2e9; font-size: 27px; font-weight: 700; }
        QLabel#subtitle { color: #aab4aa; }
        QPushButton { background: #2b342f; border: 1px solid #435047; border-radius: 6px; color: #f0f2e9; padding: 10px 14px; text-align: left; }
        QPushButton:hover { background: #35433a; border-color: #a8d35f; }
        QPushButton:pressed { background: #a8d35f; color: #11140f; }
    )");

    const QString workspaceRoot = workspaceRootFromExecutablePath();

    QWidget window;
    window.setObjectName("window");
    window.setMinimumWidth(360);

    auto *outer = new QVBoxLayout(&window);
    outer->setContentsMargins(24, 24, 24, 24);
    outer->setSpacing(16);

    auto *card = new QFrame;
    card->setObjectName("card");
    auto *content = new QVBoxLayout(card);
    content->setContentsMargins(22, 20, 22, 22);
    content->setSpacing(8);

    auto *eyebrow = new QLabel("SERENITY / PLAYGROUND");
    eyebrow->setObjectName("eyebrow");
    auto *title = new QLabel("Examples");
    title->setObjectName("title");
    auto *subtitle = new QLabel("Choose a scene to launch.");
    subtitle->setObjectName("subtitle");
    content->addWidget(eyebrow);
    content->addWidget(title);
    content->addWidget(subtitle);
    content->addSpacing(10);

    for (const auto &example : examples) {
        auto *button = new QPushButton(example.name);
        QObject::connect(button, &QPushButton::clicked, [example, workspaceRoot] {
            launch(example, workspaceRoot);
        });
        content->addWidget(button);
    }

    outer->addWidget(card);
    window.show();
#if defined(Q_OS_MACOS)
    QApplication::processEvents();
    configureTransparentTitlebar(window);
    TitlebarDragFilter dragFilter(window);
    app.installEventFilter(&dragFilter);
#else
    window.setWindowTitle("Serenity Examples");
#endif
    return app.exec();
}
