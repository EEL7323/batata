angular.module("ruServer").controller("adminLogCtrl", function ($scope, $http, $location, config, $window) {

    $scope.logs = [{
        id: 0,
        sourceId: "",
        targetId: "",
        eventType: "",
        diffCredCellphone: 0,
        diffCredTag: 0,
        toresolve: false,
        time: ""
    }];

    $scope.bigScreen = $window.innerWidth > 700 ? true : false;

    $scope.clearLogs = function () {
        $http.get(config.serverBaseUrl + "deleteLogs.php")
            .then(function (response) {
                console.log(response.data);
            });
    };

    $http.get(config.serverBaseUrl + "loadLogs.php")
        .then(function (response) {
            response.data.forEach(function (index) {
                index.time = new Date(index.time.slice(0, 4), index.time.slice(5, 7) - 1, index.time.slice(8, 10),
                    index.time.slice(11, 13), index.time.slice(14, 16), index.time.slice(17, 19), 0);
                switch (index.type) {
                    case '0':
                        index.type = "new user";
                        break;
                    case '1':
                        index.type = "del user";
                        break;
                    case '2':
                        index.type = "edit user";
                        break;
                    case '3':
                        index.type = "ru in";
                        break;
                    case '4':
                        index.type = "ru out";
                        break;
                    case '5':
                        index.type = "credit change";
                        break;
                    default: index.type = "undefined";
                }
            });
            $scope.logs = response.data;
        });    

});