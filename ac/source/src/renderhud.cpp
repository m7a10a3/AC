// renderhud.cpp: HUD rendering

#include "pch.h"
#include "cube.h"

void drawicon(Texture *tex, float x, float y, float s, int col, int row, float ts)
{
    if(tex && tex->xs == tex->ys) quad(tex->id, x, y, s, ts*col, ts*row, ts);
}

void drawequipicon(float x, float y, int col, int row, float blend)
{
    static Texture *tex = NULL;
    if(!tex) tex = textureload("packages/misc/items.png", 3);
    if(tex)
    {
        if(blend) glEnable(GL_BLEND);
        drawicon(tex, x, y, 120, col, row, 1/3.0f);
        if(blend) glDisable(GL_BLEND);
    }
}

void drawradaricon(float x, float y, float s, int col, int row)
{
    static Texture *tex = NULL;
    if(!tex) tex = textureload("packages/misc/radaricons.png", 3);
    if(tex)
    {
        glEnable(GL_BLEND);
        drawicon(tex, x, y, s, col, row, 1/4.0f);
        glDisable(GL_BLEND);
    }
}

void drawctficon(float x, float y, float s, int col, int row, float ts)
{
    static Texture *ctftex = NULL, *htftex = NULL, *ktftex = NULL;
    if(!ctftex) ctftex = textureload("packages/misc/ctficons.png", 3);
    if(!htftex) htftex = textureload("packages/misc/htficons.png", 3);
    if(!ktftex) ktftex = textureload("packages/misc/ktficons.png", 3);
    if(m_htf)
    {
        if(htftex) drawicon(htftex, x, y, s, col, row, ts);
    }
    else if(m_ktf)
    {
        if(ktftex) drawicon(ktftex, x, y, s, col, row, ts);
    }
    else
    {
        if(ctftex) drawicon(ctftex, x, y, s, col, row, ts);
    }
}

void drawvoteicon(float x, float y, int col, int row, bool noblend)
{
    static Texture *tex = NULL;
    if(!tex) tex = textureload("packages/misc/voteicons.png", 3);
    if(tex)
    {
        if(noblend) glDisable(GL_BLEND);
        drawicon(tex, x, y, 240, col, row, 1/2.0f);
        if(noblend) glEnable(GL_BLEND);
    }
}

VARP(crosshairsize, 0, 15, 50);
VARP(hidestats, 0, 1, 1);
VARP(crosshairfx, 0, 1, 1);
VARP(crosshairteamsign, 0, 1, 1);
VARP(hideradar, 0, 0, 1);
VARP(hidecompass, 0, 0, 1);
VARP(hideteam, 0, 0, 1);
VARP(radarres, 1, 64, 1024);
VARP(radarentsize, 1, 4, 64);
VARP(hidectfhud, 0, 0, 1);
VARP(hidevote, 0, 0, 1);
VARP(hidehudmsgs, 0, 0, 1);
VARP(hidehudequipment, 0, 0, 1);
VARP(hideconsole, 0, 0, 1);
VARP(hidespecthud, 0, 0, 1);
VAR(showmap, 0, 0, 1);

void drawscope()
{
    // this may need to change depending on the aspect ratio at which the scope image is drawn at
    const float scopeaspect = 4.0f/3.0f;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    static Texture *scopetex = NULL;
    if(!scopetex) scopetex = textureload("packages/misc/scope.png", 3);
    glBindTexture(GL_TEXTURE_2D, scopetex->id);
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);

    // figure out the bounds of the scope given the desired aspect ratio
    float w = min(scopeaspect*VIRTH, float(VIRTW)),
          x1 = VIRTW/2 - w/2,
          x2 = VIRTW/2 + w/2;

    glTexCoord2f(0, 0); glVertex2f(x1, 0);
    glTexCoord2f(1, 0); glVertex2f(x2, 0);
    glTexCoord2f(1, 1); glVertex2f(x2, VIRTH);
    glTexCoord2f(0, 1); glVertex2f(x1, VIRTH);

    // fill unused space with border texels
    if(x1 > 0)
    {
        glTexCoord2f(0, 0); glVertex2f(0, 0);
        glTexCoord2f(0, 0); glVertex2f(x1, 0);
        glTexCoord2f(0, 1); glVertex2f(x1, VIRTH);
        glTexCoord2f(0, 1); glVertex2f(0, VIRTH);
    }

    if(x2 < VIRTW)
    {
        glTexCoord2f(1, 0); glVertex2f(x2, 0);
        glTexCoord2f(1, 0); glVertex2f(VIRTW, 0);
        glTexCoord2f(1, 1); glVertex2f(VIRTW, VIRTH);
        glTexCoord2f(1, 1); glVertex2f(x2, VIRTH);
    }

    glEnd();
}

Texture *defaultcrosshair = NULL;
Texture *crosshairs[NUMGUNS] = { NULL }; // weapon specific crosshairs

Texture *loadcrosshairtexture(const char *c)
{
    s_sprintfd(p)("packages/misc/crosshairs/%s", c);
    Texture *crosshair = textureload(p, 3);
    if(crosshair==notexture) crosshair = textureload("packages/misc/crosshairs/default.png", 3);
    return crosshair;
}

void loadcrosshair(char *c, char *w) 
{ 
    int weapon = atoi(w);
    if(weapon>NUMGUNS) return;
    if(weapon<0 || !w[0]) defaultcrosshair = loadcrosshairtexture(c); 
    else crosshairs[weapon] = loadcrosshairtexture(c);
}

COMMAND(loadcrosshair, ARG_2STR);

void drawcrosshair(playerent *p, bool showteamwarning, int weaponspecific, color *c, float size)
{
    static Texture *teammatetex = NULL;
    if(!teammatetex) teammatetex = textureload("packages/misc/teammate.png", 3);
    
    if(!defaultcrosshair) defaultcrosshair = loadcrosshairtexture("default.png");
    Texture *crosshair = weaponspecific>=0 && crosshairs[weaponspecific] ? crosshairs[weaponspecific] : defaultcrosshair;

    if(crosshair->bpp==32) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    else glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, showteamwarning ? teammatetex->id : crosshair->id);
    glColor3ub(255,255,255);
    if(c) glColor3f(c->r, c->g, c->b);
    else if(crosshairfx)
    {
        if(showteamwarning) glColor3ub(255, 0, 0);
        else if(!m_osok)
        {
            if(p->health<=25) glColor3ub(255,0,0);
            else if(p->health<=50) glColor3ub(255,128,0);
        }
    }
    float s = size>0 ? size : (float)crosshairsize;
	float chsize = s * (p->weaponsel->type==GUN_ASSAULT && p->weaponsel->shots > 3 ? 1.4f : 1.0f) * (showteamwarning ? 2.0f : 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(VIRTW/2 - chsize, VIRTH/2 - chsize);
    glTexCoord2f(1, 0); glVertex2f(VIRTW/2 + chsize, VIRTH/2 - chsize);
    glTexCoord2f(1, 1); glVertex2f(VIRTW/2 + chsize, VIRTH/2 + chsize);
    glTexCoord2f(0, 1); glVertex2f(VIRTW/2 - chsize, VIRTH/2 + chsize);
    glEnd();
}

VARP(hidedamageindicator, 0, 0, 1);
VARP(damageindicatorsize, 0, 200, 10000);
VARP(damageindicatordist, 0, 500, 10000);
VARP(damageindicatortime, 1, 1000, 10000);
VARP(damageindicatoralpha, 1, 50, 100);
int damagedirections[4] = {0};

void updatedmgindicator(vec &attack)
{
    if(hidedamageindicator || !damageindicatorsize) return;
    float bestdist = 0.0f;
    int bestdir = -1;
    loopi(4)
    {
        vec d;
        d.x = (float)(cosf(RAD*(player1->yaw-90+(i*90))));
        d.y = (float)(sinf(RAD*(player1->yaw-90+(i*90))));
        d.z = 0.0f;
        d.add(player1->o);
        float dist = d.dist(attack);
        if(dist < bestdist || bestdir==-1)
        {
            bestdist = dist;
            bestdir = i;
        }
    }
    damagedirections[bestdir] = lastmillis+damageindicatortime;
}

void drawdmgindicator()
{
    if(!damageindicatorsize) return;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);
    float size = (float)damageindicatorsize;
    loopi(4)
    {
        if(!damagedirections[i] || damagedirections[i] < lastmillis) continue;
        float t = damageindicatorsize/(float)(damagedirections[i]-lastmillis);
        glPushMatrix();
        glColor4f(0.5f, 0.0f, 0.0f, damageindicatoralpha/100.0f);
        glTranslatef(VIRTW/2, VIRTH/2, 0);
        glRotatef(i*90, 0, 0, 1);
        glTranslatef(0, (float)-damageindicatordist, 0);
        glScalef(max(0.0f, 1.0f-t), max(0.0f, 1.0f-t), 0);

        glBegin(GL_TRIANGLES);
        glVertex3f(size/2.0f, size/2.0f, 0.0f);
        glVertex3f(-size/2.0f, size/2.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
        glPopMatrix();
    }
    glEnable(GL_TEXTURE_2D);
}

void drawequipicons(playerent *p)
{
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f+(sinf(lastmillis/100.0f)+1.0f)/2.0f);

    // health & armor
    if(p->armour) drawequipicon(620, 1650, 2, 0, false);
    drawequipicon(20, 1650, 1, 0, (p->state!=CS_DEAD && p->health<=20 && !m_osok));

    // weapons
    int c = p->weaponsel->type, r = 1;
    if(c==GUN_AKIMBO) c = GUN_PISTOL; // same icon for akimb & pistol
    if(c==GUN_GRENADE) c = r = 0;
    else if(c>2) { c -= 3; r = 2; }

    if(p->weaponsel && p->weaponsel->type>=GUN_KNIFE && p->weaponsel->type<NUMGUNS) drawequipicon(1220, 1650, c, r, (!p->weaponsel->mag && p->weaponsel->type != GUN_KNIFE && p->weaponsel->type != GUN_GRENADE)); // flowtron [second segfault]
    glEnable(GL_BLEND);
}

void drawradarent(float x, float y, float yaw, int col, int row, float iconsize, bool pulse, char *label = NULL, ...)
{
    glPushMatrix();
    if(pulse) glColor4f(1.0f, 1.0f, 1.0f, 0.2f+(sinf(lastmillis/30.0f)+1.0f)/2.0f);
    else glColor4f(1, 1, 1, 1);
    glTranslatef(x, y, 0);
    glRotatef(yaw, 0, 0, 1);
    drawradaricon(-iconsize/2.0f, -iconsize/2.0f, iconsize, col, row);
    glPopMatrix();
    if(label && showmap)
    {
        glPushMatrix();
        glEnable(GL_BLEND);
        glTranslatef(iconsize/2, iconsize/2, 0);
        glScalef(1/2.0f, 1/2.0f, 1/2.0f);
        s_sprintfdv(lbl, label);
        draw_text(lbl, (int)(x*2), (int)(y*2));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_BLEND);
        glPopMatrix();
    }
}

struct hudline : cline
{
    int type;

    hudline() : type(HUDMSG_INFO) {}
};

struct hudmessages : consolebuffer<hudline>
{
    hudmessages() : consolebuffer<hudline>(20) {}

    void addline(const char *sf)
    {
        if(conlines.length() && conlines[0].type&HUDMSG_OVERWRITE)
        {
            conlines[0].millis = totalmillis;
            conlines[0].type = HUDMSG_INFO;
            s_strcpy(conlines[0].line, sf);
        }
        else consolebuffer<hudline>::addline(sf, totalmillis);
    }
    void editline(int type, const char *sf)
    {
        if(conlines.length() && ((conlines[0].type&HUDMSG_TYPE)==(type&HUDMSG_TYPE) || conlines[0].type&HUDMSG_OVERWRITE))
        {
            conlines[0].millis = totalmillis;
            conlines[0].type = type;
            s_strcpy(conlines[0].line, sf);
        }
        else consolebuffer<hudline>::addline(sf, totalmillis).type = type;
    }
    void render()
    {
        if(!conlines.length()) return;
        glLoadIdentity();
		glOrtho(0, VIRTW*0.9f, VIRTH*0.9f, 0, -1, 1);
        int dispmillis = arenaintermission ? 6000 : 3000;
        loopi(min(conlines.length(), 3)) if(totalmillis-conlines[i].millis<dispmillis)
        {
            cline &c = conlines[i];
            int tw = text_width(c.line);
            draw_text(c.line, int(tw > VIRTW*0.9f ? 0 : (VIRTW*0.9f-tw)/2), int(((VIRTH*0.9f)/4*3)+FONTH*i+pow((totalmillis-c.millis)/(float)dispmillis, 4)*VIRTH*0.9f/4.0f));
        }
    }
};

hudmessages hudmsgs;

void hudoutf(const char *s, ...)
{
    s_sprintfdv(sf, s);
    hudmsgs.addline(sf);
    conoutf("%s", sf);
}

void hudonlyf(const char *s, ...)
{
    s_sprintfdv(sf, s);
    hudmsgs.addline(sf);
}

void hudeditf(int type, const char *s, ...)
{
    s_sprintfdv(sf, s);
    hudmsgs.editline(type, sf);
}

bool insideradar(const vec &centerpos, float radius, const vec &o)
{
    if(showmap) return !o.reject(centerpos, radius);
    return o.distxy(centerpos)<=radius;
}

bool isattacking(playerent *p) { return lastmillis-p->lastaction < 500; }

void drawradar(playerent *p, int w, int h)
{
    vec center = showmap ? vec(ssize/2, ssize/2, 0) : p->o;
    int res = showmap ? ssize : radarres;

    float worldsize = (float)ssize;
    float radarviewsize = showmap ? VIRTH : VIRTH/6;
    float radarsize = worldsize/res*radarviewsize;
    float iconsize = radarentsize/(float)res*radarviewsize;
    float coordtrans = radarsize/worldsize;
    float overlaysize = radarviewsize*4.0f/3.25f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();

    if(showmap) glTranslatef(VIRTW/2-radarviewsize/2, 0, 0);
    else
    {
        glTranslatef(VIRTW-VIRTH/28-radarviewsize-(overlaysize-radarviewsize)/2-10+radarviewsize/2, 10+VIRTH/52+(overlaysize-radarviewsize)/2+radarviewsize/2, 0);
        glRotatef(-camera1->yaw, 0, 0, 1);
        glTranslatef(-radarviewsize/2, -radarviewsize/2, 0);
    }

    extern GLuint minimaptex;

    vec centerpos(min(max(center.x, res/2.0f), worldsize-res/2.0f), min(max(center.y, res/2.0f), worldsize-res/2), 0.0f);
    if(showmap)
    {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
        quad(minimaptex, 0, 0, radarviewsize, (centerpos.x-res/2)/worldsize, (centerpos.y-res/2)/worldsize, res/worldsize);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
        circle(minimaptex, radarviewsize/2, radarviewsize/2, radarviewsize/2, centerpos.x/worldsize, centerpos.y/worldsize, res/2/worldsize);
    }
    glTranslatef(-(centerpos.x-res/2)/worldsize*radarsize, -(centerpos.y-res/2)/worldsize*radarsize, 0);

    drawradarent(p->o.x*coordtrans, p->o.y*coordtrans, p->yaw, p->state==CS_ALIVE ? (isattacking(p) ? 2 : 0) : 1, 2, iconsize, isattacking(p), colorname(p)); // local player

    loopv(players) // other players
    {
        playerent *pl = players[i];
        if(!pl || pl==p || !isteam(p->team, pl->team) || !insideradar(centerpos, res/2, pl->o)) continue;
        drawradarent(pl->o.x*coordtrans, pl->o.y*coordtrans, pl->yaw, pl->state==CS_ALIVE ? (isattacking(pl) ? 2 : 0) : 1, team_int(pl->team), iconsize, isattacking(pl), colorname(pl));
    }
    if(m_flags)
    {
        glColor4f(1.0f, 1.0f, 1.0f, (sinf(lastmillis / 100.0f) + 1.0f) / 2.0f);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        loopi(2) // flag items
        {
            flaginfo &f = flaginfos[i];
            entity *e = f.flagent;
            if(!e) continue;
            float yaw = showmap ? 0 : camera1->yaw;
            if(insideradar(centerpos, res/2, vec(e->x, e->y, centerpos.z)))
                drawradarent(e->x*coordtrans, e->y*coordtrans, yaw, m_ktf ? 2 : f.team, 3, iconsize, false); // draw bases
            if(m_ktf && f.state == CTFF_IDLE) continue;
            vec pos(0.5f-0.1f, 0.5f-0.9f, 0);
            pos.mul(iconsize/coordtrans).rotate_around_z(yaw*RAD);
            if(f.state==CTFF_STOLEN)
            {
                if(f.actor)
                {
                    pos.add(f.actor->o);
                    bool tm = i != team_int(p->team);
                    if(m_htf) tm = !tm;
                    else if(m_ktf) tm = true;
                    if(tm && insideradar(centerpos, res/2, pos))
                        drawradarent(pos.x*coordtrans, pos.y*coordtrans, yaw, 3, m_ktf ? 2 : f.team, iconsize, true); // draw near flag thief
                }
            }
            else 
            {
                pos.x += e->x;
                pos.y += e->y;
                pos.z += centerpos.z;
                if(insideradar(centerpos, res/2, pos)) drawradarent(pos.x*coordtrans, pos.y*coordtrans, yaw, 3, m_ktf ? 2 : f.team, iconsize, false); // draw on entitiy pos
            }
        }
    }

    glEnable(GL_BLEND);
    glPopMatrix();

    if(!showmap)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor3f(1, 1, 1);
        static Texture *bordertex = NULL;
        if(!bordertex) bordertex = textureload("packages/misc/compass-base.png", 3);
        quad(bordertex->id, VIRTW-10-VIRTH/28-overlaysize, 10+VIRTH/52, overlaysize, 0, 0, 1, 1);
        if(!hidecompass)
        {
            static Texture *compasstex = NULL;
            if(!compasstex) compasstex = textureload("packages/misc/compass-rose.png", 3);
            glPushMatrix();
            glTranslatef(VIRTW-10-VIRTH/28-overlaysize/2, 10+VIRTH/52+overlaysize/2, 0);
            glRotatef(-camera1->yaw, 0, 0, 1);
            quad(compasstex->id, -overlaysize/2, -overlaysize/2, overlaysize, 0, 0, 1, 1);
            glPopMatrix();
        }
    }
}

void drawteamicons(int w, int h)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1, 1, 1);
    static Texture *icons = NULL;
    if(!icons) icons = textureload("packages/misc/teamicons.png", 3);
    quad(icons->id, VIRTW-VIRTH/12-10, 10, VIRTH/12, team_int(player1->team) ? 0.5f : 0, 0, 0.49f, 1.0f);
}

void gl_drawhud(int w, int h, int curfps, int nquads, int curvert, bool underwater)
{
    playerent *p = camera1->type==ENT_PLAYER ? (playerent *)camera1 : player1;
    bool spectating = player1->isspectating();

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, VIRTW, VIRTH, 0, -1, 1);
    glEnable(GL_BLEND);

    if(underwater)
    {
        glDepthMask(GL_FALSE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4ub(hdr.watercolor[0], hdr.watercolor[1], hdr.watercolor[2], 102);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(VIRTW, 0);
        glVertex2f(VIRTW, VIRTH);
        glVertex2f(0, VIRTH);
        glEnd();
        glDepthMask(GL_TRUE);
    }

    glEnable(GL_TEXTURE_2D);

    playerent *targetplayer = playerincrosshair();
    bool menu = menuvisible();
    bool command = getcurcommand() ? true : false;

    if((p->state==CS_ALIVE || p->state==CS_EDITING) && !p->weaponsel->reloading)
    {
        bool drawteamwarning = crosshairteamsign && targetplayer && isteam(targetplayer->team, p->team) && targetplayer->state==CS_ALIVE;
        p->weaponsel->renderaimhelp(drawteamwarning);
    }

    drawdmgindicator();

    if(p->state==CS_ALIVE && !hidehudequipment) drawequipicons(p);

    if(!editmode)
    {
        glMatrixMode(GL_MODELVIEW);
        if(/*!menu &&*/ (!hideradar || showmap)) drawradar(p, w, h);
        if(!hideteam && m_teammode) drawteamicons(w, h);
        glMatrixMode(GL_PROJECTION);
    }

    char *infostr = editinfo();
    int commandh = 1570 + FONTH;
    if(command) commandh -= rendercommand(20, 1570, VIRTW);
    else if(infostr) draw_text(infostr, 20, 1570);
    else if(targetplayer) draw_text(colorname(targetplayer), 20, 1570);

    glLoadIdentity();
    glOrtho(0, VIRTW*2, VIRTH*2, 0, -1, 1);

    if(!hideconsole) renderconsole();
    if(menu) rendermenu();
    else if(command) renderdoc(40, VIRTH, max(commandh*2 - VIRTH, 0));
    if(!hidestats)
    {
        const int left = (VIRTW-225-10)*2, top = (VIRTH*7/8)*2;
        draw_textf("fps %d", left, top, curfps);
        draw_textf("lod %d", left, top+80, lod_factor());
        draw_textf("wqd %d", left, top+160, nquads);
        draw_textf("wvt %d", left, top+240, curvert);
        draw_textf("evt %d", left, top+320, xtraverts);
    }

    if(!hidevote && multiplayer(false))
    {
        extern votedisplayinfo *curvote;

        if(curvote && curvote->millis >= totalmillis)
        {
            const int left = 20*2, top = VIRTH;
            draw_textf("%s called a vote:", left, top+240, curvote->owner ? colorname(curvote->owner) : "");
            draw_textf("%s", left, top+320, curvote->desc);
            draw_textf("----", left, top+400);
            draw_textf("%d yes vs. %d no", left, top+480, curvote->stats[VOTE_YES], curvote->stats[VOTE_NO]);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glColor4f(1.0f, 1.0f, 1.0f, (sinf(lastmillis/100.0f)+1.0f) / 2.0f);
            switch(curvote->result)
            {
                case VOTE_NEUTRAL:
                    drawvoteicon(left, top, 0, 0, true);
                    draw_textf("\f3press F1/F2 to vote yes or no", left, top+560);
                    break;
                default:
                    drawvoteicon(left, top, (curvote->result-1)&1, 1, false);
                    draw_textf("\f3vote %s", left, top+560, curvote->result == VOTE_YES ? "PASSED" : "FAILED");
                    break;
            }
        }
    }

    if(!hidehudmsgs) hudmsgs.render();


    if(!hidespecthud && p->state==CS_DEAD && p->spectatemode<=SM_DEATHCAM)
    {
        glLoadIdentity();
		glOrtho(0, VIRTW*3/2, VIRTH*3/2, 0, -1, 1);
        const int left = (VIRTW*3/2)*6/8, top = (VIRTH*3/2)*3/4;
        draw_textf("SPACE to change view", left, top);
        draw_textf("SCROLL to change player", left, top+80);
    }

    // FIXME
    glLoadIdentity();
	glOrtho(0, VIRTW*3/2, VIRTH*3/2, 0, -1, 1);
    const int left = (VIRTW*3/2)*4/8, top = (VIRTH*3/2)*3/4;
    draw_textf("!TEST BUILD!", left, top);
    // FIXME

    if(!hidespecthud && spectating && player1->spectatemode!=SM_DEATHCAM)
    {
        glLoadIdentity();
		glOrtho(0, VIRTW, VIRTH, 0, -1, 1);
		draw_text("SPECTATING", VIRTW/40, VIRTH/10*7);
        if(player1->spectatemode==SM_FOLLOW1ST || player1->spectatemode==SM_FOLLOW3RD || player1->spectatemode==SM_FOLLOW3RD_TRANSPARENT)
        {
            if(players.inrange(player1->followplayercn) && players[player1->followplayercn])
            {
                s_sprintfd(name)("Player %s", players[player1->followplayercn]->name);
                draw_text(name, VIRTW/40, VIRTH/10*8);
            }
        }
    }

    if(p->state==CS_ALIVE)
    {
        glLoadIdentity();
        glOrtho(0, VIRTW/2, VIRTH/2, 0, -1, 1);

        if(!hidehudequipment)
        {
            draw_textf("%d",  90, 827, p->health);
            if(p->armour) draw_textf("%d", 390, 827, p->armour);
            if(p->weaponsel && p->weaponsel->type>=GUN_KNIFE && p->weaponsel->type<NUMGUNS) p->weaponsel->renderstats(); // flowtron
        }

        if(m_flags && !hidectfhud)
        {
            glLoadIdentity();
            glOrtho(0, VIRTW, VIRTH, 0, -1, 1);
            glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            loopi(2) // flag state
            {
                if(flaginfos[i].state == CTFF_INBASE) glDisable(GL_BLEND); else glEnable(GL_BLEND);
                drawctficon(i*120+VIRTW/4.0f*3.0f, 1650, 120, i, 0, 1/4.0f);
            }

            // big flag-stolen icon
            int ft = 0;
            if((flaginfos[0].state==CTFF_STOLEN && flaginfos[0].actor == p && flaginfos[0].ack) ||
               (flaginfos[1].state==CTFF_STOLEN && flaginfos[1].actor == p && flaginfos[1].ack && ++ft))
            {
                glColor4f(1.0f, 1.0f, 1.0f, (sinf(lastmillis/100.0f)+1.0f) / 2.0f);
                glEnable(GL_BLEND);
                drawctficon(VIRTW-225-10, VIRTH*5/8, 225, ft, 1, 1/2.0f);
                glDisable(GL_BLEND);
            }
        }
    }

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
}

void loadingscreen(const char *fmt, ...)
{
    static Texture *logo = NULL;
    if(!logo) logo = textureload("packages/misc/startscreen.png", 3);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, VIRTW, VIRTH, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0, 0, 0, 1);
    glColor3f(1, 1, 1);

    loopi(fmt ? 1 : 2)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        quad(logo->id, (VIRTW-VIRTH)/2, 0, VIRTH, 0, 0, 1);
        if(fmt)
        {
            glEnable(GL_BLEND);
            s_sprintfdlv(str, fmt, fmt);
            int w = text_width(str);
            draw_text(str, w>=VIRTW ? 0 : (VIRTW-w)/2, VIRTH*3/4);
            glDisable(GL_BLEND);
        }
        SDL_GL_SwapBuffers();
    }

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
}

static void bar(float bar, int o, float r, float g, float b)
{
    int side = 2*FONTH;
    float x1 = side, x2 = bar*(VIRTW*1.2f-2*side)+side;
    float y1 = o*FONTH;
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_STRIP);
    loopk(10)
    {
       float c = cosf(M_PI/2 + k/9.0f*M_PI), s = 1 + sinf(M_PI/2 + k/9.0f*M_PI);
       glVertex2f(x2 - c*FONTH, y1 + s*FONTH);
       glVertex2f(x1 + c*FONTH, y1 + s*FONTH);
    }
    glEnd();

    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINE_LOOP);
    loopk(10)
    {
        float c = cosf(M_PI/2 + k/9.0f*M_PI), s = 1 + sinf(M_PI/2 + k/9.0f*M_PI);
        glVertex2f(x1 + c*FONTH, y1 + s*FONTH);
    }
    loopk(10)
    {
        float c = cosf(M_PI/2 + k/9.0f*M_PI), s = 1 - sinf(M_PI/2 + k/9.0f*M_PI);
        glVertex2f(x2 - c*FONTH, y1 + s*FONTH);
    }
    glEnd();
}

void show_out_of_renderloop_progress(float bar1, const char *text1, float bar2, const char *text2)   // also used during loading
{
    c2skeepalive();

    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, VIRTW*1.2f, VIRTH*1.2f, 0, -1, 1);

    glLineWidth(3);

    if(text1)
    {
        bar(1, 1, 0.1f, 0.1f, 0.1f);
        if(bar1>0) bar(bar1, 1, 0.2f, 0.2f, 0.2f);
    }

    if(bar2>0)
    {
        bar(1, 3, 0.1f, 0.1f, 0.1f);
        bar(bar2, 3, 0.2f, 0.2f, 0.2f);
    }

    glLineWidth(1);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    if(text1) draw_text(text1, 2*FONTH, 1*FONTH + FONTH/2);
    if(bar2>0) draw_text(text2, 2*FONTH, 3*FONTH + FONTH/2);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    SDL_GL_SwapBuffers();
}

