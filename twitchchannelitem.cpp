#include "twitchchannelitem.h"

TwitchChannelItem::TwitchChannelItem(QString channelName, bool defaultConnect, QObject* parent) :
    QObject(parent),
    QStandardItem(QIcon(":/images/NOK.png"), channelName)
{
    //Set the name of the channel
    setText(channelName);

    //Activate checkboxes for every entry
    setCheckable(true);

    //Check or uncheck the checkbox based on provided value
    if ( true == defaultConnect )
    {
        setCheckState(Qt::Checked);
    }
    else
    {
        setCheckState(Qt::Unchecked);
    }
}

TwitchChannelItem::~TwitchChannelItem()
{

}

int TwitchChannelItem::type() const
{
    return 1500;
}
