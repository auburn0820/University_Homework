//
//  BodyWeight+CoreDataProperties.swift
//  
//
//  Created by νΌμμ on 2021/06/07.
//
//

import Foundation
import CoreData


extension BodyWeight {

    @nonobjc public class func fetchRequest() -> NSFetchRequest<BodyWeight> {
        return NSFetchRequest<BodyWeight>(entityName: "BodyWeight")
    }

    @NSManaged public var weight: Double
    @NSManaged public var date: Date?

}
