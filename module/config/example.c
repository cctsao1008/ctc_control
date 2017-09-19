#include <stdio.h>

#include "memmgr.h"

#include "config.h"

int main(int argc, const char *argv[])
{
    CONFIG *cfg = config_open(null);
    bool succ = config_load(cfg, "example.ini");
    int nbr_sections = config_get_nbr_sections(cfg);
    const char **section_names = config_get_sections(cfg);
    int i;
    const char *comments = config_get_comment(cfg);
    if(comments != null)
    {
        printf("#%s\n", comments);
    }
    for(i=0; i<nbr_sections; i++)
    {
        int nbr_keys = config_get_nbr_keys(cfg, section_names[i]);
        const char **key_names = config_get_keys(cfg, section_names[i]);
        int j;
        comments = config_get_section_comment(cfg, section_names[i]);
        if(comments != null)
        {
            printf("#%s\n", comments);
        }
        if(section_names[i][0] != '\0')
        {   
            printf("[%s]\n", section_names[i]);
        }
        for(j=0; j<nbr_keys; j++)
        {
            comments = config_get_key_comment(cfg, section_names[i], key_names[j]);
            if(comments != null)
            {
                printf("#%s\n", comments);
            }
            printf("  %s\t= %s\n", key_names[j], config_get_value_string(cfg, section_names[i], key_names[j], ""));
        }
        printf("\n");
    } 



    config_set_comment(cfg, "Test ini file\nHello libconfig\n##################");
    if(!config_has_section(cfg, "test"))
    {
        config_set_section_comment(cfg, "test", "This is test section");
    }
    nbr_sections = config_get_nbr_sections(cfg);
    if(nbr_sections > 0)
    {
        section_names = config_get_sections(cfg);
        if(config_get_nbr_keys(cfg, section_names[0]) > 0)
        {
            config_delete_key(cfg, section_names[0], config_get_keys(cfg, section_names[0])[0]);
        }
        config_delete_section(cfg, section_names[0]);
    }
    config_set_key_comment(cfg, "test", "testkey_string", "This is a string key");
    config_set_value_int(cfg, "test", "testkey_string", 789);
    config_set_value_string(cfg, "", "testkey_string", "hello config");
    config_set_value_int(cfg, "", "testkey_integer", 0xffff);
    config_set_value_float(cfg, "", "testkey_float", 123.456f);
    config_set_value_bool(cfg, "", "testkey_boolean", true);

    config_delete_key(cfg, "section 1", "var1");
    config_delete_key(cfg, "section 2", "var4");

    config_set_key_comment(cfg, "", "testkey_string", "hahawa...");

    config_save(cfg, "output.ini");
    config_close(cfg);

    return 0;
}

