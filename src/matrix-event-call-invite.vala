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
public class Matrix.Event.CallInvite : Matrix.Event.Call {
    /**
     * The type of session description.
     */
    public CallOfferType offer_type { get; set; default = CallOfferType.UNKNOWN; }
    /**
     * The SDP text of the session description.
     */
    public string? sdp { get; set; default = null; }

    /**
     * The time in milliseconds that the invite is valid for. Once the
     * invite age exceeds this value, clients should discard it. They
     * should also no longer show the call as awaiting an answer in
     * the UI.
     */
    public int lifetime { get; set; default = -1; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

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

        if ((node = content_root.get_member("lifetime")) != null) {
            _lifetime = (int)node.get_int();
        } else {
            warning("content.lifetime is missing from a m.call.invite event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_offer_type == CallOfferType.UNKNOWN) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.invite without a valid offer.type");
        }

        if (_sdp == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.invite without offer.sdp");
        }

        if (_lifetime < 0) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.invite without lifetime");
        }

        var content_root = json_data.get_object()
            .get_member("content").get_object();

        content_root.set_int_member("lifetime", _lifetime);

        var offer_root = new Json.Object();
        var offer_node = new Json.Node(Json.NodeType.OBJECT);
        offer_node.set_object(offer_root);

        offer_root.set_string_member(
                "type",
                _g_enum_value_to_nick(typeof(CallOfferType),
                                      _offer_type));
        offer_root.set_string_member("sdp", _sdp);

        content_root.set_member("offer", offer_node);

        base.to_json(json_data);
    }
}
