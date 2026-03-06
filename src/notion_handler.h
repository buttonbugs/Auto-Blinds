#ifndef NOTION_HANDLER
#define NOTION_HANDLER

bool get_latest_command(bool * auto_mode, int * target, String * page_id);
float download_current_status();
void upload_current_status(bool auto_mode, float angle, float target, double sun_u, double sun_v, double sun_w);

#endif