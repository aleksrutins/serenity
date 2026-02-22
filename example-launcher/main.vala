using Gtk;

struct Example {
    string name;
    string cmd;
}

const Example[] examples = {
    {"Lettuce", "serenity-example-lettuce"},
    {"Input", "serenity-example-input"},
    {"Camera", "serenity-example-camera"},
    {"Don't Look Back", "serenity-example-dontlookback"},
    {"Runtime", "serenity-runtime-test"}
};


void launch(string cmd) {
    stdout.printf("launch: %s\n", cmd);
    try {
        Pid child_pid;
        Process.spawn_async(null, {cmd}, null, SEARCH_PATH, null, out child_pid);
    } catch(GLib.Error e) {
        stdout.printf("error: %s\n", e.message);
    }
}

int main(string[] args) {
    var app = new Adw.Application("com.farthergate.Serenity", DEFAULT_FLAGS);

    app.activate.connect(() => {
        var wnd = new ApplicationWindow(app);
        wnd.title = "Serenity Examples";

        var box = new Box(VERTICAL, 6);
        box.margin_top = 10;
        box.margin_bottom = 10;
        box.margin_start = 10;
        box.margin_end = 10;


        foreach (var ex in examples) {
            var btn = new Button.with_label(ex.name);
            btn.clicked.connect(() => launch(ex.cmd));
            box.append(btn);
        }

        wnd.child = box;
        wnd.present();
    });

    return app.run(args);
}
