//
// Created by Moritz on 29.03.2020.
//

#ifndef PLANTSENSOR_LISTUIELEMENTWITHSETTINGS_H
#define PLANTSENSOR_LISTUIELEMENTWITHSETTINGS_H

#include <Arduino.h>
#include "WebUi.h"

template<typename T, typename U>   //Typen ungleich, gibt 0 zurück
struct is_same
{
    enum { value = 0 };
};

//class UiElementWithSettingsBase;
template<class T> class SettSettingsManager;
typedef struct { } BaseUiElements;

class UiElementWithSettingsBase {
    nullptr_t eepParameter;
    uint16_t uiElement;
public:
    UiElementWithSettingsBase(uint16_t uiElement) {
        uiElement = uiElement;
        next = nullptr;
        eepParameter = nullptr;
    };
    UiElementWithSettingsBase* next;
    virtual String getEepValue();
    virtual String getEepName();
    virtual void setEepParameter(String value);
    virtual void saveEepParameter(SettingsManager &settingsManager);
    virtual void loadEepParameter(SettingsManager &settingsManager);
    virtual uint16_t getUiElement();
};


template<class T>
class UiElementWithSettings : public UiElementWithSettingsBase {
    EepParameter<T> *eepParameter;

//    template<typename A>
    void _saveEepParameter(EepParameter<T> *parameter, String value) {
        this->eepParameter->value = (T)value;
    }

public:
//    template<typename T>
    UiElementWithSettings(uint16_t uiElement, EepParameter<T> *parameter) : UiElementWithSettingsBase(uiElement) {
        this->uiElement = uiElement;
        this->eepParameter = parameter;
        this->next = nullptr;
    };
    uint16_t uiElement;
    UiElementWithSettingsBase* next;

    uint16_t getUiElement() {
        return this->uiElement;
    }

    String getEepName() {
        return this->eepParameter->eepName;
    }
    String getEepValue() {
        return String(this->eepParameter->value);
    }
    EepParameter<T> *getEepParameter() {
        return this->eepParameter;
    }
    void setEepParameter(String value) {
//        Serial.print("Setting Parameter: ");
//        Serial.println(value);
        this->_saveEepParameter(this->eepParameter, value);
//        Serial.println(this->eepParameter->value);
    }
    void saveEepParameter(SettingsManager &settingsManager) {
        settingsManager.saveValue(*(this->eepParameter));
        loadEepParameter(settingsManager);
    }
    void loadEepParameter(SettingsManager &settingsManager) {
        this->eepParameter->value = settingsManager.loadValue(*(this->eepParameter));
    }


};

template<> inline void UiElementWithSettings<String>::_saveEepParameter(EepParameter<String> *param, String value) {
    param->value = value;
}
template<> inline void UiElementWithSettings<bool>::_saveEepParameter(EepParameter<bool> *param, String value) {
    param->value = value.toInt();
}
template<> inline void UiElementWithSettings<uint8_t>::_saveEepParameter(EepParameter<uint8_t> *param, String value) {
    param->value = value.toInt();
}
template<> inline void UiElementWithSettings<uint16_t>::_saveEepParameter(EepParameter<uint16_t> *param, String value) {
    param->value = value.toInt();
}
template<> inline void UiElementWithSettings<float>::_saveEepParameter(EepParameter<float> *param, String value) {
    param->value = value.toFloat();
}

class ListUiElementWithSettings {
public:

    ListUiElementWithSettings(BaseUiElements *baseUiElements) {
        this->elements = nullptr;
        this->uiElements = baseUiElements;
    };

    UiElementWithSettingsBase *getIterator() {
        return this->elements;
    }
    UiElementWithSettingsBase *getElement(uint16_t id) {
        UiElementWithSettingsBase *element = this->elements;

        while (element != nullptr) {
            if (element->getUiElement() == id) {
                return element;
            }

            element = element->next;
        }

        return nullptr;
    }

    template <typename T>
    void addElement(uint16_t uiElement, EepParameter<T> *parameter) {
        UiElementWithSettings<T> *element = new UiElementWithSettings<T>(uiElement, parameter);

        if (this->elements == nullptr) {
            this->elements = element;
        } else {
            UiElementWithSettingsBase *iterator = this->elements;

            while (iterator->next != nullptr) {
                iterator = iterator->next;
            }
            iterator->next = element;
        }
    }

    template <typename T>
    EepParameter<T> *getEepParameterFromControl(uint16_t controlId) {
        auto *element = (UiElementWithSettingsBase*)this->getIterator();
        log_d("Got iterator: %s", element->getEepName().c_str());
        while (element != nullptr) {
            if (element->getUiElement() == controlId) {
                auto e = (UiElementWithSettings<T> *) (element);
                log_d("Got Parameter: %s", e->getEepName().c_str());
                return e->getEepParameter();
            }

            element = element->next;
        }
        return nullptr;
    }

    uint16_t parentTab;
    BaseUiElements *uiElements;
private:
    UiElementWithSettingsBase *elements;
};

//template<typename T>
//void ListUiElementWithSettings::addElement(uint16_t uiElement, EepParameter<T> *parameter)

//UiElementWithSettingsBase *ListUiElementWithSettings::getElement(uint16_t id)

//UiElementWithSettingsBase *ListUiElementWithSettings::getIterator()


#endif //PLANTSENSOR_LISTUIELEMENTWITHSETTINGS_H
