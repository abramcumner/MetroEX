#pragma once

#include "../MetroTypes.h"
#include "../scripts/Script.h"
#include <array>
#include <mymath.h>
#include <variant>

class MetroReflectionReader;

struct InitData {
    uint32_t   clsid;
    uint32_t   static_data_key;
    CharString att_bone_id;
    uint16_t   id;
    uint16_t   parent_id;
    mat43T     att_offset;
    bool       att_root;

    void Serialize(MetroReflectionReader& r);
};

struct EditorProps {
    CharString caption;

    void Serialize(MetroReflectionReader& r);
};

struct uobject_static_params {
    uint16_t    version;
    EditorProps __edit;
    bool        editable;
    bool        visible_for_ai;
    bool        block_ai_los;
    bool        accept_fast_explosion;
    bool        collideable;
    float       usage_distance;

    virtual void Read(MetroReflectionReader& r, uint16_t version);
};

struct uobject {
    virtual ~uobject() {
    }

    virtual void Read(MetroReflectionReader& r);
    // на самом деле возвращает common_vss**
    virtual bool common_vss() {
        return false;
    }

    InitData   initData;
    CharString cls;
    CharString static_data;

    CharString         name;
    Flags8             oflags;
    Flags8             sflags;
    float              cull_distance;
    mat43T             pose;
    CharString         visual;
    uint16_t           dao_val;
    color4f            render_aux_val;
    MyArray<Script>    vss_ver_6;
    bool               vs_active;
    uint16_t           spatial_sector;
    uint8_t            qsave_chunk;
    MyArray<ScriptRef> commons_vs;
    MyArray<ScriptRef> removed_vs;

    uobject_static_params* param;
};

struct interest_info {
    uint16_t min_importance;
    uint16_t max_importance;
    uint8_t  interest_type;
    uint16_t duration;
    float    speed;
    float    distance;
    float    max_angle_x;
    float    max_angle_y;
    float    angle_coef;

    void Serialize(MetroReflectionReader& r);
};

struct uobject_static : public uobject {
    Flags8        flags;
    uint8_t       collision_group;
    interest_info interest;

    void Read(MetroReflectionReader& r) override;
};

struct uobject_effect : public uobject {
    AnimationString     startup_animation;
    CharString          bone_part;
    uint16_t            start_frame;
    float               speed;
    Flags8              startup_animation_flags;
    uint8_t             force_looped;
    CharString          sound;
    FloatQ8             sound_volume;
    uint8_t             sound_filter;
    CharString          particles;
    Flags8              particle_flags;
    interest_info       interest;
    MyArray<CharString> labels;

    void Read(MetroReflectionReader& r) override;
    bool common_vss() override {
        return true;
    }
};

struct centity_static_params : public uobject_static_params {
    CharString collision_sound;
    CharString collision_track;
    uint32_t   collision_interval;
    float      collision_move;
    float      attach_threshold;
    float      attach_armor;

    void Read(MetroReflectionReader& r, uint16_t version) override;
};

struct PhysicsShape {
    uint16_t bid;

    void Serialize(MetroReflectionReader& r);
};

struct PhysicsElement {
    uint16_t              root_bid;
    float                 accumulated_impulse;
    mat43T                xform;
    vec3                  velocity;
    bool                  nx_awake;
    MyArray<PhysicsShape> shapes;

    void Serialize(MetroReflectionReader& r);
};

struct PhysicsShell {
    MyArray<PhysicsElement> elements;

    void Serialize(MetroReflectionReader& r);
};

struct PhysicsJointParam {
    MyArray<uint8_t> unknown;

    void Serialize(MetroReflectionReader& r);
};

struct PhysicsJoint {
    bool              enabled;
    EntityLink        entity_src;
    CharString        bone_src;
    EntityLink        entity_dst;
    CharString        bone_dst;
    vec3              pos;
    ang3              rot;
    uint16_t          joint_type;
    PhysicsJointParam params;

    void Serialize(MetroReflectionReader& r);
};

struct centity : public uobject_effect {
    float    health;
    uint32_t dying_mask;
    Flags8   physics_flags;
    Flags8   physics_flags1;
    Flags8   physics_flags2;

    uint8_t      friend_type;
    uint8_t      reaction_type;
    CharString   fixed_bones;
    float        break_impulse_threshold;
    uint8_t      collisions_group;
    uint8_t      scene_type;
    CharString   break_particles_break;
    CharString   break_particles_death;
    CharString   break_sound_death;
    uint8_t      break_sound_death_ai_type;
    uint64_t     type_mask;
    uint32_t     ph_shell_model_src;
    uint32_t     ph_shell_skltn_src;
    uint32_t     ph_shell_skltn_bcount;
    bool         ph_shell_writed;
    PhysicsShell physics_shell;
    bool         attach_with_joint;
    PhysicsJoint joint_section;
    float        footprint_size;
    float        footprint_power;

    void Read(MetroReflectionReader& r) override;
};

struct lamp : public centity {
    void Read(MetroReflectionReader& r) override;
};

struct SlotProps {
    uint32_t slot;

    void Serialize(MetroReflectionReader& r);
};

struct inventory_item_static_params {
    SlotProps  slot;
    Flags8     flags;
    float      control_inertion_factor;
    float      speed_coef;
    float      sens_coef;
    uint32_t   sprint2run_time;
    uint32_t   run2sprint_time;
    uint32_t   slot_max_num;
    uint32_t   keepsakes_count;
    CharString active_holder_attp;
    CharString holder_attp;
    CharString holder_attp1;
    CharString holder_attp2;
    CharString active_item_attp;
    CharString item_attp;
    CharString active_holder_attp_npc;
    CharString holder_attp_npc;
    CharString holder_attp1_npc;
    CharString holder_attp2_npc;
    CharString active_item_attp_npc;
    CharString item_attp_npc;
    uint16_t   ui_tag;

    void Read(MetroReflectionReader& r, uint16_t version);
};

struct inventory_item_object_static_params : public centity_static_params {
    inventory_item_static_params inventory_item;
    CharString                   hr_class;
    float                        take_impulse;
    CharString                   take_sound;
    bool                         can_be_taken_as_child;

    void Read(MetroReflectionReader& r, uint16_t version) override;
};

struct inventory_item_object : public centity {
    // inventory_item
    Flags8   flags0;
    uint16_t trade_weight;
    uint8_t  ui_force_slot_id;
    bool     anim_simplification; // ? неизвестно в каком классе должно быть

    void Read(MetroReflectionReader& r) override;
};

struct chud_item_container_static_params {
    void Read(MetroReflectionReader& r, uint16_t version);
};

struct upgrade_item_static_params : public inventory_item_object_static_params {
    chud_item_container_static_params container;

    void Read(MetroReflectionReader& r, uint16_t version) override;
};

struct upgrade_item : public inventory_item_object {
    void Read(MetroReflectionReader& r) override;

    CharString upgrade_id;
};

struct device_upgrade_static_params : public upgrade_item_static_params {
    uint8_t menu_event;

    void Read(MetroReflectionReader& r, uint16_t version) override;
};

struct device_upgrade : public upgrade_item {};

struct player_timer_base : public device_upgrade {};

struct player_timer_static_params : public device_upgrade_static_params {};

struct player_timer : public player_timer_base {};

struct player_timer_hud_item_static_params {
    float      font_size;
    CharString font_name;
    vec4i      color;
    vec4i      color_active;
    vec4i      color_time;
    vec4i      color_vs;
    CharString light_bone;

    void Read(MetroReflectionReader& r, uint16_t version);
};

struct player_timer_hud_item_object_static_params : public player_timer_static_params {
    player_timer_hud_item_static_params hud_item;

    void Read(MetroReflectionReader& r, uint16_t version) override;
};

struct player_timer_hud_item_object : public player_timer {};

struct shooting_particles_data {
    void Read(MetroReflectionReader& r, uint16_t version);
};

struct shooting_light_data {
    void Read(MetroReflectionReader& r, uint16_t version);
};

struct shooting_weapon_data {
    void Read(MetroReflectionReader& r, uint16_t version);
};

struct weapon_item_static_params : public upgrade_item_static_params {
    void Read(MetroReflectionReader& r, uint16_t version) override;
};

struct chuditem_static_params {
    void Read(MetroReflectionReader& r, uint16_t version);
};

struct weapon_item : public upgrade_item {
    bool vr_attach;
    bool free_on_level;

    void Read(MetroReflectionReader& r) override;
};

struct uobject_vs : public uobject {
    void Read(MetroReflectionReader& r) override;
};

struct ShapeSphere {
    vec3  center;
    float radius;

    void Serialize(MetroReflectionReader& r);
};

struct ShapeBox {
    mat4 pose;
    vec3 h_size;

    void Serialize(MetroReflectionReader& r);
};

struct Shape {
    uint32_t    type;
    ShapeSphere sphere;
    ShapeBox    box;

    void Serialize(MetroReflectionReader& r);
};

struct uobject_restrictor : public uobject {
    CharString     label;
    MyArray<Shape> shapes;
    uint8_t        collisions_group;
    uint8_t        obstacle_collision_group;
    Flags8         flags0;
    uint8_t        block_ai_vision;
    uint8_t        scene_type;
    CharString     step_gmtrl;
    uint8_t        dynamic_mode;

    void Read(MetroReflectionReader& r) override;
};

struct uobject_zone : public uobject_restrictor {
    uint64_t type_mask;
    bool     type_filter_on;

    void Read(MetroReflectionReader& r) override;
};

struct uobject_interest : public uobject {
    interest_info interest;

    void Read(MetroReflectionReader& r) override;
};

struct PointLink {
    EntityLink object;
    float      weight;

    void Serialize(MetroReflectionReader& r);
};

struct uobject_aipoint : public uobject {
    PointLink  links[4];
    Flags8     ai_map;
    CharString cover_group;

    void Read(MetroReflectionReader& r) override;
};

struct PatrolState {
    CharString body_state;
    CharString anim_state;
    CharString movement_type;
    CharString weapon_state;
    CharString action;
    EntityLink target;
    uint32_t   flags;
    float      anim_state_approach_speed;
    float      approaching_accel;

    void Serialize(MetroReflectionReader& r);
};

struct patrol_point : public uobject_aipoint {
    uint32_t    min_wait_time;
    uint32_t    max_wait_time;
    PatrolState state;
    void        Read(MetroReflectionReader& r) override;
};

struct helper_text : public uobject {
    CharString text;
    CharString text_key;
    float      size;
    color4f    color;
    CharString font;
    Flags8     flags0;
    float      width;
    float      height;
    uint8_t    h_alignment;
    float      display_dist;

    void Read(MetroReflectionReader& r) override;
};

struct uobject_proxy : public uobject {
    uint16_t            slice_count;
    MyArray<EntityLink> entities;

    void Read(MetroReflectionReader& r) override;
};

struct unknown_static_params : public uobject_static_params {
    BytesArray unknown;

    void Read(MetroReflectionReader& r, uint16_t version) override;
};

struct UnknownObject : public uobject {
    BytesArray unknown;

    void Read(MetroReflectionReader& r) override;
};
