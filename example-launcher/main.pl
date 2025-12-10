#!/usr/bin/env perl
use Glib::IO;
use Glib::Object::Introspection;
use Hash::Ordered;

Glib::Object::Introspection->setup(
  basename => 'Gtk',
  version => '4.0',
  package => 'Gtk4');

Glib::Object::Introspection->setup(
  basename => 'Adw',
  version => '1',
  package => 'Adwaita');

sub launch_example {
	my ($cmd) = @_;
	my $pid = fork;
	die "failed to fork" unless defined $pid;

	if($pid == 0) {
		# child
		exec($cmd) or print STDERR "Could not exec $cmd";
	}
}

my $examples = Hash::Ordered->new(
	Lettuce => "serenity-example-lettuce",
	Input => "serenity-example-input"
);


# Create a new application
my $app =
  Adwaita::Application->new('com.farthergate.serenity.Example', 'flags-none');

# When the application is launched…
$app->signal_connect(
  activate => sub {
    my $app = shift;
    # … create a new window …
    my $win = Gtk4::ApplicationWindow->new($app);
    $win->set_title('Serenity Examples');
    my $box = Gtk4::Box->new('vertical', 6);
    $box->set_margin_start(10);
    $box->set_margin_end(10);
    $box->set_margin_top(10);
    $box->set_margin_bottom(10);

    foreach my $example ($examples->keys) {
	    my $btn = Gtk4::Button->new_with_label($example);
	    # … which closes the window when clicked
	    $btn->signal_connect(clicked => sub { launch_example($examples->get($example)); });

	    $box->append($btn);
    }

    $win->set_child($box);
    $win->present();
  }
);

# Run the application
$app->run();