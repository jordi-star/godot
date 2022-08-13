/*************************************************************************/
/*  sprite_frames.h                                                      */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef SPRITE_FRAMES_H
#define SPRITE_FRAMES_H

#include "scene/resources/texture.h"

class SpriteAnimationFrame : public RefCounted {
	GDCLASS(SpriteAnimationFrame, RefCounted);

	Ref<Texture2D> texture;
	HashMap<StringName, Vector2> anchor_offsets;

	protected:
		static void _bind_methods();
	public:
		static Dictionary anchors_to_dict(const HashMap<StringName, Vector2> &p_anchors);
		static HashMap<StringName, Vector2> dict_to_anchors(const Dictionary &p_anchors_dict);
		void set_texture(const Ref<Texture2D> &p_texture);
		Ref<Texture2D> get_texture() const;

		void set_anchor_offset(const StringName &p_anchor_name, const Vector2 &p_anchor_offset);
		_FORCE_INLINE_ Vector2 get_anchor_offset(const StringName &p_anchor_name) const {
			HashMap<StringName, Vector2>::ConstIterator E = anchor_offsets.find(p_anchor_name);
			ERR_FAIL_COND_V_MSG(!E, Vector2(), "Anchor offset '" + String(p_anchor_name) + "' doesn't exist.");
			return anchor_offsets[p_anchor_name];
		}
		Dictionary get_anchor_offsets() const;
		void set_anchor_offsets(const Dictionary &p_anchors);

		SpriteAnimationFrame();
		SpriteAnimationFrame(const Ref<Texture2D> &p_texture, const HashMap<StringName, Vector2> &p_anchors = {});
};

class SpriteFrames : public Resource {
	GDCLASS(SpriteFrames, Resource);


	struct Anim {
		double speed = 5.0;
		bool loop = true;
		Vector<Ref<SpriteAnimationFrame>> frames;
		HashMap<StringName, Vector2> anchors;
	};

	HashMap<StringName, Anim> animations;

	Array _get_animations() const;
	void _set_animations(const Array &p_animations);

protected:
	static void _bind_methods();

public:
	void add_animation(const StringName &p_anim);
	bool has_animation(const StringName &p_anim) const;
	void remove_animation(const StringName &p_anim);
	void rename_animation(const StringName &p_prev, const StringName &p_next);

	void get_animation_list(List<StringName> *r_animations) const;
	Vector<String> get_animation_names() const;

	void set_animation_speed(const StringName &p_anim, double p_fps);
	double get_animation_speed(const StringName &p_anim) const;

	void set_animation_loop(const StringName &p_anim, bool p_loop);
	bool get_animation_loop(const StringName &p_anim) const;

	void add_frame(const StringName &p_anim, const Ref<SpriteAnimationFrame> &p_frame, int p_at_pos = -1);
	int get_frame_count(const StringName &p_anim) const;
	_FORCE_INLINE_ Ref<SpriteAnimationFrame> get_frame(const StringName &p_anim, int p_idx) const {
		HashMap<StringName, Anim>::ConstIterator E = animations.find(p_anim);
		ERR_FAIL_COND_V_MSG(!E, Ref<SpriteAnimationFrame>(), "Animation '" + String(p_anim) + "' doesn't exist.");
		ERR_FAIL_COND_V(p_idx < 0, Ref<SpriteAnimationFrame>());
		if (p_idx >= E->value.frames.size()) {
			return Ref<SpriteAnimationFrame>();
		}

		return E->value.frames[p_idx];
	}

	void set_frame(const StringName &p_anim, int p_idx, const Ref<Texture2D> &p_frame) {
		HashMap<StringName, Anim>::Iterator E = animations.find(p_anim);
		ERR_FAIL_COND_MSG(!E, "Animation '" + String(p_anim) + "' doesn't exist.");
		ERR_FAIL_COND(p_idx < 0);
		if (p_idx >= E->value.frames.size()) {
			return;
		}
		E->value.frames.write[p_idx]->set_texture(p_frame);
	}
	void remove_frame(const StringName &p_anim, int p_idx);
	void clear(const StringName &p_anim);
	void clear_all();

	SpriteFrames();
};

#endif // SPRITE_FRAMES_H
