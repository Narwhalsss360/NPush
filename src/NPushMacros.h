#define GET_OnReleaseEventArgs(arg) *(OnReleaseEventArgs*)arg
#define ONRELEASE_ESR(name, args_name, defenition) EVENT_FUNCTION(name, base_##args_name) { OnReleaseEventArgs args_name = GET_OnReleaseEventArgs(base_##args_name); defenition }
#define GET_OnPushEventArgs(arg) *(OnPushEventArgs*)arg
#define ONPUSH_ESR(name, args_name, defenition) EVENT_FUNCTION(name, base_##args_name) { OnPushEventArgs args_name = GET_OnPushEventArgs(base_##args_name); defenition }