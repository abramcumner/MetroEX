#include "Entity.h"
#include "../MetroReflection.h"
#include "../scripts/Block.h"
#include "../scripts/BlockFactory.h"
#include <array>

void InitData::Serialize(MetroReflectionReader& r) {
    r.VerifyTypeInfo("class", MetroTypeGetAlias<decltype(clsid)>());
    r >> clsid;
    METRO_READ_MEMBER(r, static_data_key);
    METRO_READ_MEMBER_CHOOSE(r, att_bone_id);
    METRO_READ_MEMBER(r, id);
    METRO_READ_MEMBER(r, parent_id);
    METRO_READ_MEMBER(r, att_offset);
    METRO_READ_MEMBER(r, att_root);
}

void EditorProps::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, caption);
}

void uobject_static_params::Read(MetroReflectionReader& r, uint16_t version) {
    this->version = version;
    METRO_READ_STRUCT_MEMBER(r, __edit);
    METRO_READ_MEMBER(r, editable);
    METRO_READ_MEMBER(r, visible_for_ai);
    METRO_READ_MEMBER(r, block_ai_los);
    METRO_READ_MEMBER(r, accept_fast_explosion);
    METRO_READ_MEMBER(r, collideable);
    METRO_READ_MEMBER(r, usage_distance);
}

void uobject::Read(MetroReflectionReader& r) {
    METRO_READ_MEMBER_EXT(r, name, name);
    METRO_READ_MEMBER(r, oflags);
    METRO_READ_MEMBER(r, sflags);
    METRO_READ_MEMBER(r, cull_distance);
    r.VerifyTypeInfo("", MetroTypeGetAlias<decltype(pose)>());
    r >> pose;
    METRO_READ_MEMBER_CHOOSE(r, visual);
    METRO_READ_MEMBER(r, dao_val);
    METRO_READ_MEMBER(r, render_aux_val);
    METRO_READ_CHILD_STRUCT_ARRAY(r, vss_ver_6);
    METRO_READ_MEMBER(r, vs_active);
    METRO_READ_MEMBER(r, spatial_sector);
    METRO_READ_MEMBER(r, qsave_chunk);
    if (common_vss()) {
        METRO_READ_CHILD_STRUCT_ARRAY(r, commons_vs);
        METRO_READ_CHILD_STRUCT_ARRAY(r, removed_vs);
    }
}

void interest_info::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, min_importance);
    METRO_READ_MEMBER(r, max_importance);
    METRO_READ_MEMBER(r, interest_type);
    METRO_READ_MEMBER(r, duration);
    METRO_READ_MEMBER(r, speed);
    METRO_READ_MEMBER(r, distance);
    METRO_READ_MEMBER(r, max_angle_x);
    METRO_READ_MEMBER(r, max_angle_y);
    METRO_READ_MEMBER(r, angle_coef);
}

void uobject_static::Read(MetroReflectionReader& r) {
    uobject::Read(r);
    METRO_READ_MEMBER(r, flags);
    METRO_READ_MEMBER(r, collision_group);
    METRO_READ_CHILD_STRUCT(r, interest);
}

void uobject_effect::Read(MetroReflectionReader& r) {
    uobject::Read(r);
    METRO_READ_MEMBER(r, startup_animation);
    METRO_READ_MEMBER_EXT(r, part_str, bone_part);
    METRO_READ_MEMBER(r, start_frame);
    METRO_READ_MEMBER(r, speed);
    METRO_READ_MEMBER(r, startup_animation_flags);
    METRO_READ_MEMBER(r, force_looped);
    METRO_READ_MEMBER_CHOOSE(r, sound);
    METRO_READ_MEMBER(r, sound_volume);
    METRO_READ_MEMBER(r, sound_filter);
    METRO_READ_MEMBER(r, particle_flags);
    METRO_READ_MEMBER_CHOOSE(r, particles);
    METRO_READ_CHILD_STRUCT(r, interest);
    METRO_READ_ARRAY32_MEMBER(r, labels);
}

void centity_static_params::Read(MetroReflectionReader& r, uint16_t version) {
    uobject_static_params::Read(r, version);
    METRO_READ_MEMBER_CHOOSE(r, collision_sound);
    METRO_READ_MEMBER_CHOOSE(r, collision_track);
    METRO_READ_MEMBER(r, collision_interval);
    METRO_READ_MEMBER(r, collision_move);
    METRO_READ_MEMBER(r, attach_threshold);
    METRO_READ_MEMBER(r, attach_armor);
}

void PhysicsShape::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, bid);
}

void PhysicsElement::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, root_bid);
    METRO_READ_MEMBER(r, accumulated_impulse);
    METRO_READ_MEMBER(r, xform);
    METRO_READ_MEMBER(r, velocity);
    METRO_READ_MEMBER(r, nx_awake);
    METRO_READ_CHILD_STRUCT_ARRAY(r, shapes);
}

void PhysicsShell::Serialize(MetroReflectionReader& r) {
    METRO_READ_CHILD_STRUCT_ARRAY(r, elements);
}

void PhysicsJointParam::Serialize(MetroReflectionReader& r) {
    unknown.resize(r.GetStream().Remains());
    r.GetStream().ReadToBuffer(unknown.data(), unknown.size());
}

void PhysicsJoint::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, enabled);
    METRO_READ_MEMBER(r, entity_src);
    METRO_READ_MEMBER_EXT(r, attp_src, bone_src);
    METRO_READ_MEMBER(r, entity_dst);
    METRO_READ_MEMBER_EXT(r, attp_src, bone_dst);
    METRO_READ_MEMBER(r, pos);
    METRO_READ_MEMBER(r, rot);
    // g_physics_world->vfptr->load_joint_desc
    METRO_READ_MEMBER(r, joint_type);
    METRO_READ_CHILD_STRUCT(r, params);
}

void centity::Read(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, health);
    METRO_READ_MEMBER(r, dying_mask);
    METRO_READ_MEMBER(r, physics_flags);
    METRO_READ_MEMBER(r, physics_flags1);
    METRO_READ_MEMBER(r, physics_flags2);
    uobject_effect::Read(r);
    METRO_READ_MEMBER(r, friend_type);
    METRO_READ_MEMBER(r, reaction_type);
    METRO_READ_MEMBER_CHOOSE(r, fixed_bones); // choose_array, str_shared
    METRO_READ_MEMBER(r, break_impulse_threshold);
    METRO_READ_MEMBER(r, collisions_group);
    METRO_READ_MEMBER(r, scene_type);
    METRO_READ_MEMBER_CHOOSE(r, break_particles_break);
    METRO_READ_MEMBER_CHOOSE(r, break_particles_death);
    METRO_READ_MEMBER_CHOOSE(r, break_sound_death);
    METRO_READ_MEMBER(r, break_sound_death_ai_type);
    METRO_READ_MEMBER(r, type_mask);
    METRO_READ_MEMBER(r, ph_shell_model_src);
    METRO_READ_MEMBER(r, ph_shell_skltn_src);
    METRO_READ_MEMBER(r, ph_shell_skltn_bcount);
    METRO_READ_MEMBER(r, ph_shell_writed);
    if (ph_shell_writed) {
        METRO_READ_CHILD_STRUCT(r, physics_shell);
    }
    METRO_READ_MEMBER(r, attach_with_joint);
    if (attach_with_joint) {
        METRO_READ_CHILD_STRUCT(r, joint_section);
    }
    METRO_READ_MEMBER(r, footprint_size);
    METRO_READ_MEMBER(r, footprint_power);
}

void lamp::Read(MetroReflectionReader& r) {
    centity::Read(r);
}

void SlotProps::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, slot);
}

void inventory_item_static_params::Read(MetroReflectionReader& r, uint16_t version) {
    METRO_READ_STRUCT_MEMBER(r, slot);
    METRO_READ_MEMBER(r, flags);
    METRO_READ_MEMBER(r, control_inertion_factor);
    METRO_READ_MEMBER(r, speed_coef);
    METRO_READ_MEMBER(r, sens_coef);
    METRO_READ_MEMBER(r, sprint2run_time);
    METRO_READ_MEMBER(r, run2sprint_time);
    METRO_READ_MEMBER(r, slot_max_num);
    METRO_READ_MEMBER(r, keepsakes_count);
    METRO_READ_MEMBER_CHOOSE(r, active_holder_attp);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp1);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp2);
    METRO_READ_MEMBER_CHOOSE(r, active_item_attp);
    METRO_READ_MEMBER_CHOOSE(r, item_attp);
    METRO_READ_MEMBER_CHOOSE(r, active_holder_attp_npc);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp_npc);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp1_npc);
    METRO_READ_MEMBER_CHOOSE(r, holder_attp2_npc);
    METRO_READ_MEMBER_CHOOSE(r, active_item_attp_npc);
    METRO_READ_MEMBER_CHOOSE(r, item_attp_npc);
    METRO_READ_MEMBER(r, ui_tag);
}

void inventory_item_object_static_params::Read(MetroReflectionReader& r, uint16_t version) {
    inventory_item.Read(r, version);
    centity_static_params::Read(r, version);
    METRO_READ_MEMBER_CHOOSE(r, hr_class);
    METRO_READ_MEMBER(r, take_impulse);
    METRO_READ_MEMBER_CHOOSE(r, take_sound);
    METRO_READ_MEMBER(r, can_be_taken_as_child);
}

void inventory_item_object::Read(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, flags0);
    METRO_READ_MEMBER(r, trade_weight);
    METRO_READ_MEMBER(r, ui_force_slot_id);
    centity::Read(r);
    METRO_READ_MEMBER(r, anim_simplification);
}

void chud_item_container_static_params::Read(MetroReflectionReader& r, uint16_t version) {
}

void upgrade_item_static_params::Read(MetroReflectionReader& r, uint16_t version) {
    inventory_item_object_static_params::Read(r, version);
    container.Read(r, version);
}

void upgrade_item::Read(MetroReflectionReader& r) {
    inventory_item_object::Read(r);
    METRO_READ_MEMBER(r, upgrade_id);
}

void device_upgrade_static_params::Read(MetroReflectionReader& r, uint16_t version) {
    upgrade_item_static_params::Read(r, version);
    METRO_READ_MEMBER(r, menu_event);
}

void player_timer_hud_item_static_params::Read(MetroReflectionReader& r, uint16_t version) {
    // chud_item::static_params::load
    // player_timer_digital::static_params::load
    METRO_READ_MEMBER(r, font_size);
    METRO_READ_MEMBER_CHOOSE(r, font_name);
    METRO_READ_MEMBER(r, color);
    METRO_READ_MEMBER(r, color_active);
    METRO_READ_MEMBER(r, color_time);
    METRO_READ_MEMBER(r, color_vs);
    METRO_READ_MEMBER_CHOOSE(r, light_bone);
}

void player_timer_hud_item_object_static_params::Read(MetroReflectionReader& r, uint16_t version) {
    player_timer_static_params::Read(r, version);
    hud_item.Read(r, version);
}

void weapon_item::Read(MetroReflectionReader& r) {
    upgrade_item::Read(r);
    METRO_READ_MEMBER(r, vr_attach);
    METRO_READ_MEMBER(r, free_on_level);
}

void uobject_vs::Read(MetroReflectionReader& r) {
    uobject::Read(r);
}

void ShapeSphere::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, center);
    METRO_READ_MEMBER(r, radius);
}

void ShapeBox::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, pose);
    METRO_READ_MEMBER(r, h_size);
}

void Shape::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, type);
    switch (type) {
    case 0:
        METRO_READ_CHILD_STRUCT(r, sphere);
        break;
    case 1:
        METRO_READ_CHILD_STRUCT(r, box);
        break;
    }
}

void uobject_restrictor::Read(MetroReflectionReader& r) {
    uobject::Read(r);
    METRO_READ_MEMBER(r, label);
    METRO_READ_CHILD_STRUCT_ARRAY(r, shapes);
    METRO_READ_MEMBER(r, collisions_group);
    METRO_READ_MEMBER(r, obstacle_collision_group);
    METRO_READ_MEMBER(r, flags0);
    METRO_READ_MEMBER(r, block_ai_vision);
    METRO_READ_MEMBER(r, scene_type);
    METRO_READ_MEMBER_CHOOSE(r, step_gmtrl);
    METRO_READ_MEMBER(r, dynamic_mode);
}

void uobject_zone::Read(MetroReflectionReader& r) {
    uobject_restrictor::Read(r);
    METRO_READ_MEMBER(r, type_mask);
    METRO_READ_MEMBER(r, type_filter_on);
}

void uobject_interest::Read(MetroReflectionReader& r) {
    uobject::Read(r);
    METRO_READ_CHILD_STRUCT(r, interest);
}

void PointLink::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, object);
    METRO_READ_MEMBER(r, weight);
}

void uobject_aipoint::Read(MetroReflectionReader& r) {
    uobject::Read(r);
    for (int i = 0; i != 4; i++) {
        char buf[10];
        sprintf(buf, "link_%d", i);
        r.ReadStruct(buf, links[i], true);
    }
    METRO_READ_MEMBER(r, ai_map);
    METRO_READ_MEMBER_CHOOSE(r, cover_group);
}

void PatrolState::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, body_state);
    METRO_READ_MEMBER(r, anim_state);
    METRO_READ_MEMBER(r, movement_type);
    METRO_READ_MEMBER(r, weapon_state);
    METRO_READ_MEMBER(r, action);
    METRO_READ_MEMBER(r, target);
    METRO_READ_MEMBER(r, flags);
    METRO_READ_MEMBER(r, anim_state_approach_speed);
    METRO_READ_MEMBER(r, approaching_accel);
}

void patrol_point::Read(MetroReflectionReader& r) {
    uobject_aipoint::Read(r);
    METRO_READ_MEMBER(r, min_wait_time);
    METRO_READ_MEMBER(r, max_wait_time);
    state.Serialize(r);
}

void helper_text::Read(MetroReflectionReader& r) {
    uobject::Read(r);
    METRO_READ_MEMBER(r, text);
    METRO_READ_MEMBER_CHOOSE(r, text_key);
    METRO_READ_MEMBER(r, size);
    METRO_READ_MEMBER(r, color);
    METRO_READ_MEMBER_CHOOSE(r, font);
    METRO_READ_MEMBER(r, flags0);
    METRO_READ_MEMBER(r, width);
    METRO_READ_MEMBER(r, height);
    METRO_READ_MEMBER(r, h_alignment);
    METRO_READ_MEMBER(r, display_dist);
}

void uobject_proxy::Read(MetroReflectionReader& r) {
    uobject::Read(r);
    METRO_READ_MEMBER(r, slice_count);
    METRO_READ_CHILD_STRUCT_ARRAY(r, entities);
}

void unknown_static_params::Read(MetroReflectionReader& r, uint16_t version) {
    this->version = version;

    unknown.resize(r.GetStream().Remains());
    r.GetStream().ReadToBuffer(unknown.data(), unknown.size());
}

void UnknownObject::Read(MetroReflectionReader& r) {
    unknown.resize(r.GetStream().Remains());
    r.GetStream().ReadToBuffer(unknown.data(), unknown.size());
}
