/* pvCopy.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author Marty Kraimer
 * @date 2013.04
 */
#ifndef PVCOPY_H
#define PVCOPY_H
#include <string>
#include <stdexcept>
#include <memory>

#include <shareLib.h>

#include <pv/pvAccess.h>
#include <pv/pvDatabase.h>

namespace epics { namespace pvDatabase { 

class PVCopy;
typedef std::tr1::shared_ptr<PVCopy> PVCopyPtr;
class PVCopyMonitor;
typedef std::tr1::shared_ptr<PVCopyMonitor> PVCopyMonitorPtr;
class PVCopyMonitorRequester;
typedef std::tr1::shared_ptr<PVCopyMonitorRequester> PVCopyMonitorRequesterPtr;

struct CopyNode;
typedef std::tr1::shared_ptr<CopyNode> CopyNodePtr;
struct CopyRecordNode;
typedef std::tr1::shared_ptr<CopyRecordNode> CopyRecordNodePtr;
struct CopyStructureNode;
typedef std::tr1::shared_ptr<CopyStructureNode> CopyStructureNodePtr;


class epicsShareClass PVCopy : 
    public std::tr1::enable_shared_from_this<PVCopy>
{
public:
    POINTER_DEFINITIONS(PVCopy);
    static PVCopyPtr create(
        PVRecordPtr const &pvRecord,
        epics::pvData::PVStructurePtr const &pvRequest,
        epics::pvData::String const & structureName);
    virtual ~PVCopy(){}
    virtual void destroy();
    PVRecordPtr getPVRecord();
    epics::pvData::StructureConstPtr getStructure();
    epics::pvData::PVStructurePtr createPVStructure();
    std::size_t getCopyOffset(PVRecordFieldPtr const  &recordPVField);
    std::size_t getCopyOffset(
        PVRecordStructurePtr const  &recordPVStructure,
        PVRecordFieldPtr const  &recordPVField);
    PVRecordFieldPtr getRecordPVField(std::size_t structureOffset);
    void initCopy(
        epics::pvData::PVStructurePtr const  &copyPVStructure,
        epics::pvData::BitSetPtr const  &bitSet);
    void updateCopySetBitSet(
        epics::pvData::PVStructurePtr const  &copyPVStructure,
        epics::pvData::BitSetPtr const  &bitSet);
    void updateCopyFromBitSet(
        epics::pvData::PVStructurePtr const  &copyPVStructure,
        epics::pvData::BitSetPtr const  &bitSet);
    void updateRecord(
        epics::pvData::PVStructurePtr const  &copyPVStructure,
        epics::pvData::BitSetPtr const  &bitSet);
    PVCopyMonitorPtr createPVCopyMonitor(
        PVCopyMonitorRequesterPtr const  &pvCopyMonitorRequester);
    epics::pvData::PVStructurePtr getOptions(
        epics::pvData::PVStructurePtr const &copyPVStructure,std::size_t fieldOffset);
    epics::pvData::String dump();
private:
    void dump(
        epics::pvData::String *builder,
        CopyNodePtr const &node,
        int indentLevel);
    PVCopyPtr getPtrSelf()
    {
        return shared_from_this();
    }

    PVRecordPtr pvRecord;
    epics::pvData::StructureConstPtr structure;
    CopyNodePtr headNode;
    epics::pvData::PVStructurePtr cacheInitStructure;
private:
    PVCopy(PVRecordPtr const &pvRecord);
    friend class PVCopyMonitor;
    bool init(epics::pvData::PVStructurePtr const &pvRequest);
    epics::pvData::String dump(
        epics::pvData::String const &value,
        CopyNodePtr const &node,
        int indentLevel);
    epics::pvData::String getFullName(
        epics::pvData::PVStructurePtr const &pvFromRequest,
        epics::pvData::String  const &nameFromRecord);
    epics::pvData::PVStructurePtr getSubStructure(
        epics::pvData::PVStructurePtr const &pvFromRequest,
        epics::pvData::String  const &nameFromRecord);
    epics::pvData::PVStructurePtr getOptions(
        epics::pvData::PVStructurePtr const &pvFromRequest,
        epics::pvData::String  const &nameFromRecord);
    epics::pvData::StructureConstPtr createStructure(
        epics::pvData::PVStructurePtr const &pvRecord,
        epics::pvData::PVStructurePtr const &pvFromRequest);
    epics::pvData::FieldConstPtr createField(
        epics::pvData::PVStructurePtr const &pvRecord,
        epics::pvData::PVStructurePtr const &pvFromRequest);
    CopyNodePtr createStructureNodes(
        PVRecordStructurePtr const &pvRecordStructure,
        epics::pvData::PVStructurePtr const &pvFromRequest,
        epics::pvData::PVFieldPtr const &pvFromField);
    void referenceImmutable(
        epics::pvData::PVFieldPtr const &pvField,
        CopyNodePtr const & node);
    void referenceImmutable(
        epics::pvData::PVFieldPtr const &copyPVField,
        PVRecordFieldPtr const &recordPVField);
    void updateStructureNodeSetBitSet(
        epics::pvData::PVStructurePtr const &pvCopy,
        CopyStructureNodePtr const &structureNode,
        epics::pvData::BitSetPtr const &bitSet);
    void updateSubFieldSetBitSet(
        epics::pvData::PVFieldPtr const &pvCopy,
        PVRecordFieldPtr const &pvRecord,
        epics::pvData::BitSetPtr const &bitSet);
    void updateStructureNodeFromBitSet(
        epics::pvData::PVStructurePtr const &pvCopy,
        CopyStructureNodePtr const &structureNode,
        epics::pvData::BitSetPtr const &bitSet,
        bool toCopy,
        bool doAll);
    void updateSubFieldFromBitSet(
        epics::pvData::PVFieldPtr const &pvCopy,
        PVRecordFieldPtr const &pvRecordField,
        epics::pvData::BitSetPtr const &bitSet,
        bool toCopy,
        bool doAll);
    CopyRecordNodePtr getCopyOffset(
        CopyStructureNodePtr const &structureNode,
        PVRecordFieldPtr const &recordPVField);
    CopyRecordNodePtr getRecordNode(
        CopyStructureNodePtr const &structureNode,
        std::size_t structureOffset);
    
};

class epicsShareClass PVCopyMonitor :
    public PVListener,
    public std::tr1::enable_shared_from_this<PVCopyMonitor>
{
public:
    POINTER_DEFINITIONS(PVCopyMonitor);
    virtual ~PVCopyMonitor();
    virtual void destroy();
    void startMonitoring(
        epics::pvData::BitSetPtr const &changeBitSet,
        epics::pvData::BitSetPtr const &overrunBitSet);
    void stopMonitoring();
    void switchBitSets(
        epics::pvData::BitSetPtr const &newChangeBitSet,
        epics::pvData::BitSetPtr const &newOverrunBitSet);
    // following are PVListener methods
    virtual void detach(PVRecordPtr const & pvRecord);
    virtual void dataPut(PVRecordFieldPtr const & pvRecordField);
    virtual void dataPut(
        PVRecordStructurePtr const & requested,
        PVRecordFieldPtr const & pvRecordField);
    virtual void beginGroupPut(PVRecordPtr const & pvRecord);
    virtual void endGroupPut(PVRecordPtr const & pvRecord);
    virtual void unlisten(PVRecordPtr const & pvRecord);
private:
    void addListener(CopyNodePtr const & node);
    CopyNodePtr findNode(
        CopyNodePtr const & node,
        PVRecordFieldPtr const & pvRecordField);
    PVCopyMonitorPtr getPtrSelf()
    {
        return shared_from_this();
    }
    PVCopyMonitor(
        PVRecordPtr const &pvRecord,
        CopyNodePtr const &headNode,
        PVCopyPtr const &pvCopy,
        PVCopyMonitorRequesterPtr const &pvCopyMonitorRequester);
    friend class PVCopy;
    PVRecordPtr pvRecord;
    CopyNodePtr headNode;
    PVCopyPtr pvCopy;
    PVCopyMonitorRequesterPtr pvCopyMonitorRequester;
    epics::pvData::BitSetPtr changeBitSet;
    epics::pvData::BitSetPtr overrunBitSet;
    bool isGroupPut;
    bool dataChanged;
    bool isMonitoring;
    epics::pvData::Mutex mutex;
};

class epicsShareClass PVCopyMonitorRequester
{
public:
    POINTER_DEFINITIONS(PVCopyMonitorRequester);
    virtual ~PVCopyMonitorRequester() {}
    virtual void dataChanged() = 0;
    virtual void unlisten() = 0;
};

}}

#endif  /* PVCOPY_H */
