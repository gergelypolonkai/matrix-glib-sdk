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
 * This event is sent by the caller when they wish to establish a
 * call.
 */
public class Matrix.Event.CallInvite : Matrix.Event.Room {
    /**
     * A unique identifer for the call.
     */
    public string? call_id { get; set; default = null;}

    /**
     * The type of session description.
     */
    public CallOfferType? offer_type { get; set; default = null; }
    /**
     * The SDP text of the session description.
     */
    public string? sdp { get; set; default = null; }

    /**
     * The version of the VoIP specification this message adheres to.
     */
    public int? version { get; set; default = null; }

    /**
     * The time in milliseconds that the invite is valid for. Once the
     * invite age exceeds this value, clients should discard it. They
     * should also no longer show the call as awaiting an answer in
     * the UI.
     */
    public int? lifetime { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("call_id")) != null) {            _call_id = node.get_string();
        } else {
            warning("content.call_id is missing from a m.call.invite event");
        }

        if ((node = content_root.get_member("offer")) != null) {
            var offer_node = node.get_object();

            if ((node = offer_node.get_member("type")) != null) {
                CallOfferType? typ = (CallOfferType?)_g_enum_nick_to_value(
                        typeof(CallOfferType), node.get_string());

                if (typ != null) {
                    _offer_type = typ;
                } else {
                    _offer_type = CallOfferType.UNKNOWN;
                    warning("Unknown value %s in content.offer.type of a m.call.invite event",
                            node.get_string());
                }
            } else {
                warning("content.offer.type is missing from a m.call.invite event");
            }

            if ((node = offer_node.get_member("sdp")) != null) {
                _sdp = node.get_string();
            } else {
                warning("content.offer.sdp is missing from a m.call.invite event");
            }
        }

        if ((node = content_root.get_member("version")) != null) {
            _version = (int)node.get_int();
        } else {
            warning("content.version is missing from a m.call.invite event");
        }

        if ((node = content_root.get_member("lifetime")) != null) {
            _lifetime = (int)node.get_int();
        } else {
            warning("content.lifetime is missing from a m.call.invite event");
        }

        base.from_json(json_data);
    }
}
