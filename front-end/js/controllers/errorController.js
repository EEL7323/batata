angular.module("ruServer").controller("errorCtrl", function ($scope, $rootScope) {

    // Variables declaration or attribution

    $rootScope.showNavMenu = false;

    // Get the indexes of title in order to read the error and show it in the error page
    var _index1 = $rootScope.phpError.indexOf("<title>");
    var _index2 = $rootScope.phpError.indexOf("</title>");
    var _splitError = $rootScope.phpError.slice(_index1 + 7, _index2);
    $scope.phpError = _splitError;
});