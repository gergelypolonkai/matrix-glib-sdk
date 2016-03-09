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
 * This event is sent by callers after sending an invite and by the
 * callee after answering. Its purpose is to give the other party
 * additional ICE candidates to try using to communicate.
 */
public class Matrix.Event.CallCandidates : Matrix.Event.Call {
    public struct Candidate {
        string? sdp_mid;     /// The SDP media type this candidate is
                             /// intended for.
        int? sdp_line_index; /// The index of the SDP 'm' line this
                             /// candidate is intended for.
        string? candidate;   /// The SDP 'a' line of the candidate.
    }

    /**
     * The list of candidates.
     */
    public List<Candidate?>? candidates {
        get {
            return _candidates;
        }

        set {
            _candidates = value.copy();
        }

        default = null;
    }

    private List<Candidate?>? _candidates;

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("candidates")) != null) {
            node.get_array().foreach_element((ary, idx, cand_node) => {
                    var cand_root = cand_node.get_object();
                    var cand = Candidate();

                    if ((node = cand_root.get_member("sdpMid")) != null) {
                        cand.sdp_mid = node.get_string();
                    } else {
                        warning("sdpMid is missing from a candidate of a m.call.candidates event");
                    }

                    if ((node = cand_root.get_member("sdpMLineIndex")) != null) {
                        cand.sdp_line_index = (int)node.get_int();
                    } else {
                        warning("sdpMLineIndex is missing from a candidate of a m.call.candidates event");
                    }

                    if ((node = cand_root.get_member("candidate")) != null) {
                        cand.candidate = node.get_string();
                    } else {
                        warning("candidate is missing from a candidate of a m.call.candidates event");
                    }

                    _candidates.prepend(cand);
                });
        } else {
            warning("content.candidates is missing from a m.call.candidates event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if ((_candidates == null) || (_candidates.length() < 1)) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.candidates event without candidates");
        }

        var content_root = json_data.get_object()
            .get_member("content").get_object();

        var cands = new Json.Array();

        foreach (var entry in _candidates) {
            if (entry.sdp_mid == null) {
                throw new Matrix.Error.INCOMPLETE(
                        "Won't generate a m.call.candidates event with a missing sdpMid for candidates");
            }

            if (entry.sdp_line_index == null) {
                throw new Matrix.Error.INCOMPLETE(
                        "Won't generate a m.call.candidates event with a missing sdpMLineIndex for candidates");
            }

            if (entry.candidate == null) {
                throw new Matrix.Error.INCOMPLETE(
                        "Won't generate a m.call.candidates event with a missing candidate for candidates");
            }

            var cand_obj = new Json.Object();
            var cand_node = new Json.Node(Json.NodeType.OBJECT);
            cand_node.set_object(cand_obj);

            cand_obj.set_string_member("sdpMid", entry.sdp_mid);
            cand_obj.set_int_member("sdpMLineIndex", entry.sdp_line_index);
            cand_obj.set_string_member("candidate", entry.candidate);

            cands.add_element(cand_node);
        }

        if (cands.get_length() < 1) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.candidates event with empty candidates list");
        }

        var cands_node = new Json.Node(Json.NodeType.ARRAY);
        cands_node.set_array(cands);
        content_root.set_member("candidates", cands_node);

        base.to_json(json_data);
    }
}
