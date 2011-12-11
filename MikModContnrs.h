//---------------------------------------------------------------------------
#ifndef MikModContnrsH
#define MikModContnrsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "MikMod.h"
//---------------------------------------------------------------------------
namespace Mikmod
{

/**
 * A template for storing a list of generic elements.
 */
template <class T>class TObjectList : public System::Classes::TList
{
    typedef System::Classes::TList inherited;

private:
    bool FOwnsObjects;

protected:
    /**
     * Responds when items are added, removed, or extracted from the list.
     * Notify is called automatically when the items in the collection change.
     * @param Ptr Points to item affected by the change.
     * @param Action Indicates the change.
     */
    virtual void __fastcall Notify(void * Ptr, System::Classes::TListNotification Action)
    {
        if(Action == lnDeleted && OwnsObjects)
        {
            delete (T)Ptr;
        }
        inherited::Notify(Ptr, Action);
    }

    /**
     * Returns a specified item in the collection.
     * GetItem is the protected read implementation of the Items property.
     * @param Index Position of an item.
     * @return A pointer to the item at Index.
     */
    T __fastcall GetItem(int Index)
    {
      return (T)inherited::Items[Index];
    }

    /**
     * Represents the abstract setter method for the Item property.
     * SetItem is the setter method for the Item property.
     * @param Index Position of an item.
     * @param AObject The object to set.
     */
    void __fastcall SetItem(int Index, T AObject)
    {
        inherited::Items[Index] = AObject;
    }

public:
    /**
     * Constructor.
     * @param AOwnsObjects Value to initialize OwnsObjects.
     *        When no value is specified, TObjectList is initialized with OwnsObjects set to true.
     */
    __fastcall TObjectList(bool AOwnsObjects = true) :
        System::Classes::TList(),
        FOwnsObjects(AOwnsObjects)
    {

    }

    /**
     * Destructor.
     */
    inline __fastcall virtual ~TObjectList(void)
    {

    }

    /**
     * Add adds a given item to the end of the list. You can add NULL.
     * An OnNotify event occurs indicating an entry was added to the list.
     * @param AObject The item to add.
     * @return The index of the item added to the list.
     */
    int __fastcall Add(T AObject)
    {
        return inherited::Add(AObject);
    }

    /**
     * Remove removes the first instance of AObject in the list, returning its zero-based index.
     * If AObject is not in the list, this function returns -1.
     * An OnNotify event occurs indicating an entry was removed from the list.
     * @param AObject The item to remove.
     * @return The index of the item removed from the list or -1 if AObject is not found.
     */
    int __fastcall Remove(T AObject)
    {
        return inherited::Remove(AObject);
    }

    /**
     * Extract removes the entry Item from the list, returning this value. If Item is not in the list, it returns the default value of its type T.
     * An OnNotify event occurs indicating an entry was removed from the list.
     * Extract is similar to Remove except for the event code indicating an element was extracted rather than removed and is provided so items may be removed without freeing.
     * @param Item The item to extract.
     * @return A pointer to the extracted value.
     */
    T __fastcall Extract(T Item)
    {
        return (T)inherited::Extract(Item);
    }

    /**
     * ExtractItem removes an item from a list. After the item is removed, the index positions of all the objects that follow it are moved up and Count is decremented.
     * In descendent classes, ExtractItem also calls the notify method specifying the value of the removed item. This allows descendent classes to perform a proper cleanup with stored values.
     * @param Item The item to extract.
     * @param ADirection The direction to start the search.
     * @return A pointer to the extracted value.
     */
    T __fastcall ExtractItem(T Item, System::Classes::TList::TDirection ADirection)
    {
        return (T)inherited::ExtractItem(Item, ADirection);
    }

    /**
     * The RemoveItem method removes a specified item from the list.
     * If the item is not found, -1 is returned.
     * @param AObject The item to search for.
     * @param ADirection The direction to start the search.
     * @return The index of the removed item in the list or -1 if AObject is not found.
     */
    int __fastcall RemoveItem(T AObject, System::Classes::TList::TDirection ADirection)
    {
        return inherited::RemoveItem(AObject, ADirection);
    }

    /**
     * IndexOf searches for the list element AObject using a linear search.
     * The method returns the zero-based index of the first entry equal to AObject.
     * If not found, it returns -1.
     * @param AObject The item to search for.
     * @return The index of the item found in the list or -1 if AObject is not found.
     */
    int __fastcall IndexOf(T AObject)
    {
        return inherited::IndexOf(AObject);
    }

    /**
     * IndexOfItem searches for the list element AObject using a linear search.
     * The method returns the zero-based index of the first entry equal to AObject.
     * If not found, it returns -1.
     * @param AObject The item to search for.
     * @param ADirection The direction to start the search.
     * @return The index of the item found in the list or -1 if AObject is not found.
     */
    int __fastcall IndexOfItem(T AObject, System::Classes::TList::TDirection ADirection)
    {
        return inherited::IndexOfItem(AObject, ADirection);
    }

    /**
     * Finds the first instance of a specified class in the list.
     * FindInstanceOf returns the index of the first instance of AClass appearing after AStartAt in the Items array.
     * If no instance of the specified class is found, FindInstanceOf returns -1.
     * @param AClass The class to find.
     * @param AExact If AExact is true, FindInstanceOf returns instances only of AClass itself, ignoring instances of descendant classes.
     * @param AStartAt The start position for searching.
     * @return The index of the item found in the list or -1 if not found.
     */
    int __fastcall FindInstanceOf(System::TClass AClass, bool AExact, int AStartAt)
    {
        int Result = -1;

        for(int i = AStartAt; i < Count; ++i)
        {
            if( (AExact && (Items[i]->ClassType() == AClass)) ||
                (!AExact &&  Items[i]->InheritsFrom(AClass)))
            {
                Result = i;
                break;
            }
        }
        return Result;
    }

    /**
     * Adds an object to the Items array at the position specified by Index.
     * Call Insert to add Item to the middle of the Items array.
     * The Index parameter is a zero-based index, so the first position in the array has an index of 0.
     * Insert adds the item at the indicated position, shifting the item that previously occupied that position, and all subsequent items, up.
     * Insert increments Count and, if necessary, allocates memory by increasing the value of Capacity.
     * To replace a NULL pointer in the array with a new item, without growing the Items array, set the Items property.
     * @param Index Position where to add the item.
     * @param AObject The item to insert.
     */
    void __fastcall Insert(int Index, T AObject)
    {
        inherited::Insert(Index, AObject);
    }

    /**
     * Call First to retrieve the first pointer in the Items array.
     * @return A pointer to the first item.
     */
    T __fastcall First(void)
    {
        return (T)inherited::First();
    }

    /**
     * Call Last to retrieve the last pointer in the Items array.
     * @return A pointer to the last item.
     */
    T __fastcall Last(void)
    {
        return (T)inherited::Last();
    }

    /**
     * Indexer operator for the Items in the List.
     * @return A pointer to the corresponding Item.
     */
    T operator[](int Index)
    {
        return (T)Items[Index];
    }

    /**
     * Allows TObjectList to free objects when they are deleted from the list or the list is destroyed.
     * OwnsObjects allows TObjectList to control the memory of its objects.
     * If OwnsObjects is true (the default),
     * calling Delete or Remove frees the deleted object in addition to removing it from the list.
     * calling Clear frees all the objects in the list in addition to emptying the list.
     * calling the destructor frees all the objects in the list in addition to destroying the TObjectList itself.
     * assigning a new value to an index in Items frees the object that previously occupied that position in the list.
     * Even if OwnsObjects is true, the Extract method can be used to remove objects from the list without freeing them.
     */
    __property bool OwnsObjects = {read=FOwnsObjects, write=FOwnsObjects, nodefault};
    /**
     * Use Items to obtain a pointer to a specific object in the array.
     * The Index parameter indicates the index of the object, where 0 is the index of the first object, 1 is the index of the second object, and so on.
     * Set Items to change the reference at a specific location.
     * Use Items with the Count property to iterate through all of the objects in the list.
     */
    __property T Items[int Index] = {read=GetItem, write=SetItem/*, default*/};
};


/**
 * A class for storing a list of TVoice.
 */
class TVoiceList : public TObjectList<TVoice*>
{
    typedef TObjectList<TVoice*> inherited;

public:
    /**
     * Constructor.
     * @param AOwnsObjects Value to initialize OwnsObjects.
     *        When no value is specified, TVoiceList is initialized with OwnsObjects set to true.
     */
    __fastcall TVoiceList(bool AOwnsObjects = true) : inherited(AOwnsObjects) { }
    /**
     * Destructor.
     */
    inline __fastcall virtual ~TVoiceList(void) { }
};

}   /* namespace Mikmod */

using namespace Mikmod;

//---------------------------------------------------------------------------
#endif
