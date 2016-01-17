
#pragma once

module Pera
{

interface News
{
    void NewMsg(string time);
};
interface PcIdToWsServer
{
    void TellClientId(string strClientId);
};
};
