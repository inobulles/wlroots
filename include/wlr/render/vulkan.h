/*
 * This an unstable interface of wlroots. No guarantees are made regarding the
 * future consistency of this API.
 */
#ifndef WLR_USE_UNSTABLE
#error "Add -DWLR_USE_UNSTABLE to enable unstable wlroots features"
#endif

#ifndef WLR_RENDER_VULKAN_H
#define WLR_RENDER_VULKAN_H

#include <vulkan/vulkan_core.h>
#include <wlr/render/wlr_renderer.h>

struct wlr_vk_image_attribs {
	VkImage image;
	VkImageLayout layout;
	VkFormat format;
};

struct wlr_renderer *wlr_vk_renderer_create_with_drm_fd(int drm_fd);

VkInstance wlr_vk_renderer_get_instance(struct wlr_renderer *renderer);
VkPhysicalDevice wlr_vk_renderer_get_physical_device(struct wlr_renderer *renderer);
VkDevice wlr_vk_renderer_get_device(struct wlr_renderer *renderer);
uint32_t wlr_vk_renderer_get_queue_family(struct wlr_renderer *renderer);

bool wlr_renderer_is_vk(const struct wlr_renderer *wlr_renderer);
bool wlr_texture_is_vk(const struct wlr_texture *texture);

void wlr_vk_texture_get_image_attribs(struct wlr_texture *texture,
	struct wlr_vk_image_attribs *attribs);
bool wlr_vk_texture_has_alpha(struct wlr_texture *texture);

/*
 * Called before code that accesses this texture from a different renderer's queue.
 */
bool wlr_vk_texture_begin_external(struct wlr_texture *texture,
	struct wlr_vk_image_attribs *attribs);

/*
 * Called before code that accesses this buffer from a different renderer's queue.
 * The image is returned in the VK_IMAGE_LAYOUT_COLOR_ATTACHEMENT_OPTIMAL layout and must remain so.
 */
bool wlr_vk_buffer_begin_external(struct wlr_renderer *renderer,
	struct wlr_buffer *buffer, struct wlr_vk_image_attribs *attribs);

/*
 * Called after work that accesses textures and buffers from a different renderer's queue is submitted.
 *
 * If signal_timeline is non-NULL, signal_point is signalled once that work has completed, and should be set as the output's wait timeline.
 * Otherwise, the buffers' implicit fences are set if the device supports it
 * Otherwise, this blocks until the work is done.
 */
bool wlr_vk_renderer_end_external(struct wlr_renderer *renderer,
	struct wlr_drm_syncobj_timeline *signal_timeline, uint64_t signal_point);

#endif

