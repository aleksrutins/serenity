using namespace serenity::input;

auto key_code = [](char c) {
	if(c >= 'a') c = (c - 'a') + 'A';

	return (int)(c - 'A' + 4);
};

lua["key"] = [key_code](char c) {
	return keyboard::state()[key_code(c)];
};

lua["keyboard"] = lua.create_table_with(
	"state", keyboard::state,
	"code", key_code,
	"special", lua.create_table_with()
);