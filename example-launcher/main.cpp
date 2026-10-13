#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>
#include <QPalette>
#include <QProcess>
#include <QPushButton>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>

namespace {

struct Example {
    const char *name;
    const char *executable;
    const char *script;
};

constexpr Example examples[] = {
    {"Lettuce", SERENITY_LETTUCE_EXECUTABLE, nullptr},
    {"Input", SERENITY_INPUT_EXECUTABLE, nullptr},
    {"Camera", SERENITY_CAMERA_EXECUTABLE, nullptr},
    {"Don't Look Back", SERENITY_DONTLOOKBACK_EXECUTABLE, nullptr},
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
    window.setWindowTitle("Serenity Examples");
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
    return app.exec();
}
