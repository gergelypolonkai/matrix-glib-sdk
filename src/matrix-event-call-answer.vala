/*
 * This file is part of matrix-glib-sdk
 *
 * matrix-glib-sdk is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

/**
 * This event is sent by the callee when they wish to answer the call.
 */
public class Matrix.Event.CallAnswer : Matrix.Event.Room {
    /**
     * The ID of the call this event relates to.
     */
    public string? call_id { get; set; default = null; }

    /**
     * The type of session description.
     */
    public CallAnswerType? answer_type { get; set; default = null; }

    /**
     * The SDP text of the session description.
     */
    public string? answer_sdp { get; set; default = null; }

    /**
     * The version of the VoIP specification this messages adheres to.
     */
    public int? version { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("call_id")) != null) {
            _call_id = node.get_string();
        } else {
            warning("content.call_id is missing from a m.call.answer event");
        }

        if ((node = content_root.get_member("answer")) != null) {
            var answer_root = node.get_object();

            if ((node = answer_root.get_member("type")) != null) {
                CallAnswerType? typ = (CallAnswerType?)_g_enum_nick_to_value(
                        typeof(CallAnswerType), node.get_string());

                if (typ != null) {
                    _answer_type = typ;
                } else {
                    _answer_type = CallAnswerType.UNKNOWN;
                    warning("Unknown value %s for content.answer.type in a m.call.answer event",
                            node.get_string());
                }
            } else {
                warning("content.answer.type is missing from a m.call.answer event");
            }

            if ((node = answer_root.get_member("sdp")) != null) {
                _answer_sdp = node.get_string();
            } else {
                warning("content.answer.sdp is missing from a m.call.answer event");
            }
        }

        if ((node = content_root.get_member("version")) != null) {
            _version = (int)node.get_int();
        } else {
            warning("content.version is missing from a m.call.answer event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_version == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.answer event without version");
        }

        if (_call_id == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.answer event without call_id");
        }

        if (_answer_type == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.answer event without answer.type");
        }

        if (_answer_sdp == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.answer event without answer.sdp");
        }

        var content_root = json_data.get_object()
            .get_member("content").get_object();

        content_root.set_string_member("call_id", _call_id);
        content_root.set_int_member("version", _version);

        var answer_root = new Json.Object();
        var answer_node = new Json.Node(Json.NodeType.OBJECT);
        answer_node.set_object(answer_root);

        answer_root.set_string_member("type",
                                      _g_enum_value_to_nick(typeof(CallAnswerType),
                                                            _answer_type));
        answer_root.set_string_member("sdp", _answer_sdp);

        content_root.set_member("answer", answer_node);

        base.to_json(json_data);
    }
}
