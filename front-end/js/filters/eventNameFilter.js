angular.module("ruServer").filter("eventName", function () {
    var _selectName = function (input) {
        switch (input) {
            case '0':
                return "new user";
                break;
            case '1':
                return "del user";
                break;
            case '2':
                return "edit user";
                break;
            case '3':
                return "ru in";
                break;
            case '4':
                return "ru out";
                break;
            case '5':
                return "credit change";
                break;
            case '6':
                return "clear logs";
                break;
            default: return "undefined";
        }
    };

    return _selectName;

});