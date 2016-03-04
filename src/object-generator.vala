/*
 * This file is part of matrix-glib-sdk
 *
 * matrix-glib-sdk is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This one specific file is licensed under the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * matrix-glib-sdk is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with matrix-glib-sdk. If not, see
 * <http://www.gnu.org/licenses/>.
 */
int main (string[] args)
{
    try {
        var directory = File.new_for_path("../schemas");

        var enumerator = directory.enumerate_children(
                FileAttribute.STANDARD_NAME, 0);

        FileInfo file_info;

        while ((file_info = enumerator.next_file()) != null) {
            var filename = file_info.get_name();

            if (filename.has_prefix("m.")) {
                File schema_file = directory.resolve_relative_path(filename);
                var parser = new Json.Parser();
                string? description = null;

                stdout.printf("Processing %s…\n", filename);

                parser.load_from_stream(schema_file.read());
                var root = parser.get_root().get_object();

                var event_type = root.get_member("type").get_string();

                stdout.printf("Type: %s\n", event_type);

                if (event_type == "object") {
                    if (filename.index_of_char('#') == -1) {
                        Json.Node? node;

                        if ((node = root.get_member("description")) != null) {
                            description = node.get_string();
                        }
                    } else {
                        stdout.printf("Event-subtype, will implement later\n");
                    }
                } else {
                    stdout.printf("Won’t process non-objects!\n");
                }

                stdout.printf("\n");
            }
        }
    } catch (Error e) {
        stderr.printf("Error: %s\n", e.message);

        return 1;
    }

    return 0;
}
