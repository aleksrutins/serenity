#!/usr/bin/env -S gjs -m

import Gtk from "gi://Gtk?version=4.0"
import Gio from "gi://Gio"
import Adw from "gi://Adw"

const launch = cmd => Gio.Subprocess.new([cmd], Gio.SubprocessFlags.STDOUT_PIPE | Gio.SubprocessFlags.STDERR_PIPE);

const app = Adw.Application.new('com.farthergate.serenity.Example', Gio.Application.DEFAULT_FLAGS);

const examples = [
  ['Lettuce', 'serenity-example-lettuce'],
  ['Input', 'serenity-example-input'],
  ['Camera', 'serenity-example-camera'],
  ["Don't Look Back", 'serenity-example-dontlookback'],
  ['Runtime', 'serenity-runtime-test']
]

app.connect('activate', () => {
  const wnd = Gtk.ApplicationWindow.new(app);
  wnd.set_title('Serenity Examples')
  const box = new Gtk.Box({
    'orientation': Gtk.Orientation.VERTICAL,
    'spacing': 6,
    'margin-top': 10,
    'margin-bottom': 10,
    'margin-start': 10,
    'margin-end': 10
  })
  for(const [title, cmd] of examples) {
    const btn = Gtk.Button.new_with_label(title);
    btn.connect('clicked', () => launch(cmd));
    box.append(btn);
  }
  wnd.set_child(box);
  wnd.present()
})

app.run([]);
